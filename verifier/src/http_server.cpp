#include "http_server.h"

#include <algorithm>
#include <atomic>
#include <cctype>
#include <cstring>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  using pact_socket_t = SOCKET;
  #define PACT_INVALID_SOCKET INVALID_SOCKET
  #define pact_close_socket closesocket
#else
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/select.h>
  #include <sys/socket.h>
  #include <sys/time.h>
  #include <unistd.h>
  using pact_socket_t = int;
  #define PACT_INVALID_SOCKET (-1)
  #define pact_close_socket ::close
#endif

namespace pact_verifier {
  namespace http {

    namespace {
      // Bound how much a client can make us buffer, so a malformed or hostile
      // request cannot exhaust memory.
      constexpr size_t MAX_HEADER_BYTES = 64 * 1024;
      constexpr size_t MAX_BODY_BYTES = 8 * 1024 * 1024;

#ifdef _WIN32
      struct WinsockGuard {
        WinsockGuard() {
          WSADATA data;
          WSAStartup(MAKEWORD(2, 2), &data);
        }
        ~WinsockGuard() { WSACleanup(); }
      };

      void ensure_winsock() {
        static WinsockGuard guard;
        (void) guard;
      }
#else
      void ensure_winsock() {}
#endif

      std::string trim(const std::string& value) {
        size_t start = value.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
          return "";
        }
        size_t end = value.find_last_not_of(" \t\r\n");
        return value.substr(start, end - start + 1);
      }

      std::string to_lower(std::string value) {
        std::transform(value.begin(), value.end(), value.begin(),
          [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return value;
      }

      std::string url_decode(const std::string& value) {
        std::string result;
        result.reserve(value.size());
        for (size_t i = 0; i < value.size(); i++) {
          if (value[i] == '+') {
            result.push_back(' ');
          } else if (value[i] == '%' && i + 2 < value.size() &&
                     std::isxdigit(static_cast<unsigned char>(value[i + 1])) &&
                     std::isxdigit(static_cast<unsigned char>(value[i + 2]))) {
            result.push_back(static_cast<char>(std::stoi(value.substr(i + 1, 2), nullptr, 16)));
            i += 2;
          } else {
            result.push_back(value[i]);
          }
        }
        return result;
      }

      void parse_query(const std::string& query, std::unordered_map<std::string, std::string>& into) {
        size_t pos = 0;
        while (pos < query.size()) {
          size_t next = query.find('&', pos);
          if (next == std::string::npos) {
            next = query.size();
          }
          std::string pair = query.substr(pos, next - pos);
          size_t eq = pair.find('=');
          if (eq == std::string::npos) {
            if (!pair.empty()) {
              into[url_decode(pair)] = "";
            }
          } else {
            into[url_decode(pair.substr(0, eq))] = url_decode(pair.substr(eq + 1));
          }
          pos = next + 1;
        }
      }

      const char* status_text(unsigned int status) {
        switch (status) {
          case 200: return "OK";
          case 201: return "Created";
          case 202: return "Accepted";
          case 204: return "No Content";
          case 301: return "Moved Permanently";
          case 302: return "Found";
          case 400: return "Bad Request";
          case 401: return "Unauthorized";
          case 403: return "Forbidden";
          case 404: return "Not Found";
          case 405: return "Method Not Allowed";
          case 409: return "Conflict";
          case 413: return "Payload Too Large";
          case 415: return "Unsupported Media Type";
          case 422: return "Unprocessable Entity";
          case 431: return "Request Header Fields Too Large";
          case 500: return "Internal Server Error";
          case 501: return "Not Implemented";
          case 503: return "Service Unavailable";
          default: return "Unknown";
        }
      }

      bool send_all(pact_socket_t socket, const char* data, size_t length) {
        size_t sent = 0;
        while (sent < length) {
#ifdef _WIN32
          int written = ::send(socket, data + sent, static_cast<int>(length - sent), 0);
#else
          ssize_t written = ::send(socket, data + sent, length - sent, 0);
#endif
          if (written <= 0) {
            return false;
          }
          sent += static_cast<size_t>(written);
        }
        return true;
      }

      void write_response(pact_socket_t socket, const Response& response) {
        std::ostringstream out;
        out << "HTTP/1.1 " << response.status << " " << status_text(response.status) << "\r\n";
        if (!response.content_type.empty()) {
          out << "Content-Type: " << response.content_type << "\r\n";
        }
        for (const auto& header : response.headers) {
          out << header.first << ": " << header.second << "\r\n";
        }
        out << "Content-Length: " << response.body.size() << "\r\n";
        out << "Connection: close\r\n\r\n";
        out << response.body;

        const std::string payload = out.str();
        send_all(socket, payload.data(), payload.size());
      }

      /**
       * Reads a complete request off the socket. Returns false if the request was
       * malformed, oversized or the connection dropped.
       */
      bool read_request(pact_socket_t socket, Request& request, unsigned int& error_status) {
        std::string buffer;
        char chunk[4096];
        size_t header_end = std::string::npos;

        while (true) {
#ifdef _WIN32
          int received = ::recv(socket, chunk, static_cast<int>(sizeof(chunk)), 0);
#else
          ssize_t received = ::recv(socket, chunk, sizeof(chunk), 0);
#endif
          if (received <= 0) {
            return false;
          }
          buffer.append(chunk, static_cast<size_t>(received));
          header_end = buffer.find("\r\n\r\n");
          if (header_end != std::string::npos) {
            break;
          }
          if (buffer.size() > MAX_HEADER_BYTES) {
            error_status = 431;
            return false;
          }
        }

        std::istringstream headers(buffer.substr(0, header_end));
        std::string line;
        if (!std::getline(headers, line)) {
          error_status = 400;
          return false;
        }

        std::istringstream request_line(trim(line));
        std::string target;
        std::string version;
        if (!(request_line >> request.method >> target >> version)) {
          error_status = 400;
          return false;
        }

        size_t query_start = target.find('?');
        if (query_start == std::string::npos) {
          request.path = url_decode(target);
        } else {
          request.path = url_decode(target.substr(0, query_start));
          request.query = target.substr(query_start + 1);
          parse_query(request.query, request.query_params);
        }

        while (std::getline(headers, line)) {
          size_t colon = line.find(':');
          if (colon == std::string::npos) {
            continue;
          }
          request.headers[to_lower(trim(line.substr(0, colon)))] = trim(line.substr(colon + 1));
        }

        size_t content_length = 0;
        auto length_header = request.headers.find("content-length");
        if (length_header != request.headers.end()) {
          try {
            long long parsed = std::stoll(length_header->second);
            if (parsed < 0 || static_cast<unsigned long long>(parsed) > MAX_BODY_BYTES) {
              error_status = 413;
              return false;
            }
            content_length = static_cast<size_t>(parsed);
          } catch (const std::exception&) {
            error_status = 400;
            return false;
          }
        }

        request.body = buffer.substr(header_end + 4);
        while (request.body.size() < content_length) {
#ifdef _WIN32
          int received = ::recv(socket, chunk, static_cast<int>(sizeof(chunk)), 0);
#else
          ssize_t received = ::recv(socket, chunk, sizeof(chunk), 0);
#endif
          if (received <= 0) {
            return false;
          }
          request.body.append(chunk, static_cast<size_t>(received));
        }
        request.body.resize(content_length);

        return true;
      }
    }

    struct Server::Impl {
      pact_socket_t listener = PACT_INVALID_SOCKET;
      std::atomic<bool> running{false};
      std::thread worker;
      Handler handler;
      uint16_t port = 0;
      std::string host;

      void serve() {
        while (running.load()) {
          fd_set read_set;
          FD_ZERO(&read_set);
          FD_SET(listener, &read_set);

          timeval timeout{};
          timeout.tv_sec = 0;
          timeout.tv_usec = 100000;

#ifdef _WIN32
          int ready = ::select(0, &read_set, nullptr, nullptr, &timeout);
#else
          int ready = ::select(listener + 1, &read_set, nullptr, nullptr, &timeout);
#endif
          if (ready <= 0) {
            continue;
          }

          pact_socket_t connection = ::accept(listener, nullptr, nullptr);
          if (connection == PACT_INVALID_SOCKET) {
            continue;
          }

          Request request;
          Response response;
          unsigned int error_status = 400;
          if (read_request(connection, request, error_status)) {
            if (handler) {
              try {
                handler(request, response);
              } catch (const std::exception& e) {
                response.status = 500;
                response.content_type = "text/plain";
                response.body = std::string("Request handler threw an exception: ") + e.what();
              } catch (...) {
                response.status = 500;
                response.content_type = "text/plain";
                response.body = "Request handler threw an unknown exception";
              }
            } else {
              response.status = 501;
              response.content_type = "text/plain";
              response.body = "No handler configured";
            }
            write_response(connection, response);
          } else if (error_status != 0) {
            Response error;
            error.status = error_status;
            error.content_type = "text/plain";
            error.body = status_text(error_status);
            write_response(connection, error);
          }

          pact_close_socket(connection);
        }
      }
    };

    Server::Server() : impl(std::make_unique<Impl>()) {}

    Server::~Server() {
      stop();
    }

    void Server::set_handler(Handler handler) {
      impl->handler = std::move(handler);
    }

    bool Server::start(uint16_t port, const std::string& host) {
      if (impl->running.load()) {
        return true;
      }

      ensure_winsock();

      impl->listener = ::socket(AF_INET, SOCK_STREAM, 0);
      if (impl->listener == PACT_INVALID_SOCKET) {
        return false;
      }

      int reuse = 1;
      ::setsockopt(impl->listener, SOL_SOCKET, SO_REUSEADDR,
        reinterpret_cast<const char*>(&reuse), sizeof(reuse));

      sockaddr_in address{};
      address.sin_family = AF_INET;
      address.sin_port = htons(port);
      if (::inet_pton(AF_INET, host.c_str(), &address.sin_addr) != 1) {
        pact_close_socket(impl->listener);
        impl->listener = PACT_INVALID_SOCKET;
        return false;
      }

      if (::bind(impl->listener, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0 ||
          ::listen(impl->listener, 16) != 0) {
        pact_close_socket(impl->listener);
        impl->listener = PACT_INVALID_SOCKET;
        return false;
      }

      sockaddr_in bound{};
#ifdef _WIN32
      int bound_len = sizeof(bound);
#else
      socklen_t bound_len = sizeof(bound);
#endif
      if (::getsockname(impl->listener, reinterpret_cast<sockaddr*>(&bound), &bound_len) != 0) {
        pact_close_socket(impl->listener);
        impl->listener = PACT_INVALID_SOCKET;
        return false;
      }

      impl->port = ntohs(bound.sin_port);
      impl->host = host;
      impl->running.store(true);
      impl->worker = std::thread([this] { impl->serve(); });

      return true;
    }

    void Server::stop() {
      if (!impl->running.exchange(false)) {
        return;
      }
      if (impl->worker.joinable()) {
        impl->worker.join();
      }
      if (impl->listener != PACT_INVALID_SOCKET) {
        pact_close_socket(impl->listener);
        impl->listener = PACT_INVALID_SOCKET;
      }
      impl->port = 0;
    }

    bool Server::is_running() const {
      return impl->running.load();
    }

    uint16_t Server::get_port() const {
      return impl->port;
    }

    std::string Server::get_url() const {
      return "http://" + impl->host + ":" + std::to_string(impl->port);
    }
  }
}
