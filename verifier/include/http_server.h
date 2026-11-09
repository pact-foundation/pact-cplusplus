#pragma once

/**
 * Minimal, dependency-free HTTP/1.1 server used to host the provider state
 * change endpoint required by Pact verification.
 *
 * It is deliberately small (single connection at a time, `Connection: close`)
 * because the Pact verifier drives the provider sequentially. It is also useful
 * for standing up a stub provider in your own tests, but it is not intended to
 * be a general purpose production web server.
 */

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace pact_verifier {
  namespace http {

    /**
     * An incoming HTTP request.
     */
    struct Request {
      /** HTTP method, upper case (GET, POST, ...) */
      std::string method;
      /** Request path, with any query string removed */
      std::string path;
      /** Raw query string (without the leading '?') */
      std::string query;
      /** Request body. Empty if the request had no body */
      std::string body;
      /** Request headers. Header names are lower cased */
      std::unordered_map<std::string, std::string> headers;
      /** URL decoded query parameters */
      std::unordered_map<std::string, std::string> query_params;
    };

    /**
     * The response to send back. Handlers mutate this in place.
     */
    struct Response {
      /** HTTP status code */
      unsigned int status = 200;
      /** Response body */
      std::string body;
      /** Value for the Content-Type header. Omitted if empty */
      std::string content_type = "application/json";
      /** Any additional response headers */
      std::unordered_map<std::string, std::string> headers;
    };

    /**
     * Callback invoked for every request the server receives.
     */
    using Handler = std::function<void(const Request&, Response&)>;

    /**
     * A small HTTP server that runs on a background thread.
     */
    class Server {
      public:
        Server();
        ~Server();

        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;

        /**
         * Sets the handler invoked for each request. Must be called before start().
         */
        void set_handler(Handler handler);

        /**
         * Binds and starts listening on the given host and port, then serves requests
         * on a background thread. Passing port 0 will allocate a free port, which can
         * be read back with get_port().
         *
         * Returns false if the socket could not be bound.
         */
        bool start(uint16_t port = 0, const std::string& host = "127.0.0.1");

        /**
         * Stops the server and joins the background thread. Safe to call more than once.
         */
        void stop();

        /**
         * True while the server is accepting requests.
         */
        bool is_running() const;

        /**
         * The port the server is bound to, or 0 if it is not running.
         */
        uint16_t get_port() const;

        /**
         * The base URL of the server, for example "http://127.0.0.1:52344".
         */
        std::string get_url() const;

      private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };
  }
}
