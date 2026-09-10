#include "verifier.h"

#include <cstring>
#include <cctype>
#include <iostream>
#include <mutex>
#include <stdexcept>

#include <nlohmann/json.hpp>

#ifndef PACT_CPP_VERIFIER_VERSION
  #define PACT_CPP_VERIFIER_VERSION "0.0.0"
#endif

using json = nlohmann::json;

namespace pact_verifier {

  namespace {
    LevelFilter to_level_filter(LogLevel level) {
      switch (level) {
        case LogLevel::Off: return LevelFilter_Off;
        case LogLevel::Error: return LevelFilter_Error;
        case LogLevel::Warn: return LevelFilter_Warn;
        case LogLevel::Info: return LevelFilter_Info;
        case LogLevel::Debug: return LevelFilter_Debug;
        case LogLevel::Trace: return LevelFilter_Trace;
      }
      return LevelFilter_Info;
    }

    /** The FFI treats a NULL pointer as "use the default", an empty string does not */
    const char* or_null(const std::string& value) {
      return value.empty() ? nullptr : value.c_str();
    }

    /**
     * Looks up a handler by key under the given mutex, falling back to the default handler.
     * Shared by ProviderStateServer::Impl and MessageProviderServer::Impl.
     */
    template <typename Handler>
    Handler lookup_handler(std::mutex& lock, const std::unordered_map<std::string, Handler>& handlers,
        const Handler& default_handler, const std::string& key) {
      std::lock_guard<std::mutex> guard(lock);
      auto found = handlers.find(key);
      return found != handlers.end() ? found->second : default_handler;
    }

    std::vector<const char*> as_c_array(const std::vector<std::string>& values) {
      std::vector<const char*> result;
      result.reserve(values.size());
      for (const auto& value : values) {
        result.push_back(value.c_str());
      }
      return result;
    }

    /** Takes ownership of a string returned from the FFI and frees it */
    std::string take_string(const char* value) {
      if (value == nullptr) {
        return "";
      }
      std::string result(value);
      pactffi_free_string(const_cast<char*>(value));
      return result;
    }

    const char BASE64_ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string base64_encode(const std::string& input) {
      std::string output;
      output.reserve(((input.size() + 2) / 3) * 4);
      for (size_t i = 0; i < input.size(); i += 3) {
        unsigned int block = static_cast<unsigned char>(input[i]) << 16;
        size_t remaining = input.size() - i;
        if (remaining > 1) {
          block |= static_cast<unsigned char>(input[i + 1]) << 8;
        }
        if (remaining > 2) {
          block |= static_cast<unsigned char>(input[i + 2]);
        }
        output.push_back(BASE64_ALPHABET[(block >> 18) & 0x3F]);
        output.push_back(BASE64_ALPHABET[(block >> 12) & 0x3F]);
        output.push_back(remaining > 1 ? BASE64_ALPHABET[(block >> 6) & 0x3F] : '=');
        output.push_back(remaining > 2 ? BASE64_ALPHABET[block & 0x3F] : '=');
      }
      return output;
    }

    std::string base64_decode(const std::string& input) {
      std::string output;
      unsigned int buffer = 0;
      int bits = 0;
      for (char c : input) {
        const char* found = std::strchr(BASE64_ALPHABET, c);
        if (c == '=' || found == nullptr || c == '\0') {
          continue;
        }
        buffer = (buffer << 6) | static_cast<unsigned int>(found - BASE64_ALPHABET);
        bits += 6;
        if (bits >= 8) {
          bits -= 8;
          output.push_back(static_cast<char>((buffer >> bits) & 0xFF));
        }
      }
      return output;
    }
  }

  void init() {
    pactffi_init("LOG_LEVEL");
  }

  bool log_to_stdout(LogLevel level) {
    return pactffi_log_to_stdout(to_level_filter(level)) == 0;
  }

  bool log_to_stderr(LogLevel level) {
    return pactffi_log_to_stderr(to_level_filter(level)) == 0;
  }

  bool log_to_file(const std::string& file_name, LogLevel level) {
    return pactffi_log_to_file(file_name.c_str(), to_level_filter(level)) == 0;
  }

  bool log_to_buffer(LogLevel level) {
    return pactffi_log_to_buffer(to_level_filter(level)) == 0;
  }

  ////////////////////////////////////
  // ProviderStateParams
  ////////////////////////////////////

  ProviderStateParams::ProviderStateParams() : params_json("{}") {}

  ProviderStateParams::ProviderStateParams(const std::string& json_text)
    : params_json(json_text.empty() ? "{}" : json_text) {
    // Qualified, because the unqualified name resolves to the json() member function here
    nlohmann::json parsed = nlohmann::json::parse(params_json, nullptr, false);
    if (parsed.is_discarded() || !parsed.is_object()) {
      params_json = "{}";
      return;
    }
    for (auto& entry : parsed.items()) {
      params[entry.key()] = entry.value().is_string()
        ? entry.value().get<std::string>()
        : entry.value().dump();
    }
  }

  bool ProviderStateParams::contains(const std::string& name) const {
    return params.find(name) != params.end();
  }

  std::optional<std::string> ProviderStateParams::get(const std::string& name) const {
    auto entry = params.find(name);
    if (entry == params.end()) {
      return std::nullopt;
    }
    return entry->second;
  }

  std::string ProviderStateParams::get_or(const std::string& name, const std::string& default_value) const {
    return get(name).value_or(default_value);
  }

  std::optional<long long> ProviderStateParams::get_int(const std::string& name) const {
    auto value = get(name);
    if (!value) {
      return std::nullopt;
    }
    try {
      size_t consumed = 0;
      long long result = std::stoll(*value, &consumed);
      if (consumed != value->size()) {
        return std::nullopt;
      }
      return result;
    } catch (const std::exception&) {
      return std::nullopt;
    }
  }

  std::optional<double> ProviderStateParams::get_double(const std::string& name) const {
    auto value = get(name);
    if (!value) {
      return std::nullopt;
    }
    try {
      size_t consumed = 0;
      double result = std::stod(*value, &consumed);
      if (consumed != value->size()) {
        return std::nullopt;
      }
      return result;
    } catch (const std::exception&) {
      return std::nullopt;
    }
  }

  std::optional<bool> ProviderStateParams::get_bool(const std::string& name) const {
    auto value = get(name);
    if (!value) {
      return std::nullopt;
    }
    if (*value == "true") {
      return true;
    }
    if (*value == "false") {
      return false;
    }
    return std::nullopt;
  }

  const std::unordered_map<std::string, std::string>& ProviderStateParams::values() const {
    return params;
  }

  const std::string& ProviderStateParams::json() const {
    return params_json;
  }

  ////////////////////////////////////
  // ProviderStateServer
  ////////////////////////////////////

  struct ProviderStateServer::Impl {
    std::string path;
    http::Server server;
    std::unordered_map<std::string, StateHandlerWithValues> handlers;
    StateHandlerWithValues default_handler;
    bool unknown_state_is_error = true;
    std::mutex lock;

    explicit Impl(std::string endpoint_path) : path(std::move(endpoint_path)) {
      if (path.empty() || path.front() != '/') {
        path = "/" + path;
      }
    }

    void handle(const http::Request& request, http::Response& response) {
      if (request.path != path) {
        response.status = 404;
        response.body = R"({"error":"Not found"})";
        return;
      }

      std::string state;
      std::string action = "setup";
      std::string params_json = "{}";

      if (!request.body.empty()) {
        json body = json::parse(request.body, nullptr, false);
        if (body.is_discarded() || !body.is_object()) {
          response.status = 400;
          response.body = R"({"error":"Request body was not a JSON object"})";
          return;
        }
        if (body.contains("state") && body["state"].is_string()) {
          state = body["state"].get<std::string>();
        }
        if (body.contains("action") && body["action"].is_string()) {
          action = body["action"].get<std::string>();
        }
        if (body.contains("params") && body["params"].is_object()) {
          params_json = body["params"].dump();
        }
      } else {
        // With `body = false` the verifier sends the state data as query parameters
        auto state_param = request.query_params.find("state");
        if (state_param != request.query_params.end()) {
          state = state_param->second;
        }
        auto action_param = request.query_params.find("action");
        if (action_param != request.query_params.end()) {
          action = action_param->second;
        }
        json params = json::object();
        for (const auto& entry : request.query_params) {
          if (entry.first != "state" && entry.first != "action") {
            params[entry.first] = entry.second;
          }
        }
        params_json = params.dump();
      }

      if (state.empty()) {
        response.status = 200;
        response.body = "{}";
        return;
      }

      ProviderStateRequest state_request;
      state_request.state = state;
      state_request.action = action == "teardown" ? StateAction::Teardown : StateAction::Setup;
      state_request.params = ProviderStateParams(params_json);

      StateHandlerWithValues handler = lookup_handler(lock, handlers, default_handler, state);

      if (!handler) {
        if (unknown_state_is_error) {
          response.status = 500;
          response.body = json({{"error", "No handler registered for provider state '" + state + "'"}}).dump();
        } else {
          response.status = 200;
          response.body = "{}";
        }
        return;
      }

      try {
        std::string values = handler(state_request);
        response.status = 200;
        response.body = values.empty() ? "{}" : values;
      } catch (const std::exception& e) {
        response.status = 500;
        response.body = json({{"error", std::string("Provider state handler failed: ") + e.what()}}).dump();
      } catch (...) {
        response.status = 500;
        response.body = json({{"error", "Provider state handler failed with an unknown exception"}}).dump();
      }
    }
  };

  ProviderStateServer::ProviderStateServer(std::string path)
    : impl(std::make_unique<Impl>(std::move(path))) {
    impl->server.set_handler([this](const http::Request& request, http::Response& response) {
      impl->handle(request, response);
    });
  }

  ProviderStateServer::~ProviderStateServer() {
    stop();
  }

  void ProviderStateServer::add_state_handler(const std::string& state, StateHandler handler) {
    add_state_handler_with_values(state, [handler](const ProviderStateRequest& request) {
      handler(request);
      return std::string();
    });
  }

  void ProviderStateServer::add_state_handler_with_values(const std::string& state, StateHandlerWithValues handler) {
    std::lock_guard<std::mutex> guard(impl->lock);
    impl->handlers[state] = std::move(handler);
  }

  void ProviderStateServer::set_default_handler(StateHandler handler) {
    set_default_handler_with_values([handler](const ProviderStateRequest& request) {
      handler(request);
      return std::string();
    });
  }

  void ProviderStateServer::set_default_handler_with_values(StateHandlerWithValues handler) {
    std::lock_guard<std::mutex> guard(impl->lock);
    impl->default_handler = std::move(handler);
  }

  void ProviderStateServer::set_unknown_state_is_error(bool is_error) {
    impl->unknown_state_is_error = is_error;
  }

  bool ProviderStateServer::start(uint16_t port, const std::string& host) {
    return impl->server.start(port, host);
  }

  void ProviderStateServer::stop() {
    impl->server.stop();
  }

  bool ProviderStateServer::is_running() const {
    return impl->server.is_running();
  }

  uint16_t ProviderStateServer::get_port() const {
    return impl->server.get_port();
  }

  std::string ProviderStateServer::get_url() const {
    return impl->server.get_url() + impl->path;
  }

  ////////////////////////////////////
  // ProviderMessage
  ////////////////////////////////////

  ProviderMessage::ProviderMessage(std::string body, std::string content_type)
    : message_body(std::move(body)), message_content_type(std::move(content_type)) {}

  ProviderMessage& ProviderMessage::with_metadata(const std::string& key, const std::string& value) {
    message_metadata[key] = json(value).dump();
    return *this;
  }

  ProviderMessage& ProviderMessage::with_json_metadata(const std::string& key, const std::string& json_value) {
    message_metadata[key] = json_value;
    return *this;
  }

  const std::string& ProviderMessage::body() const {
    return message_body;
  }

  const std::string& ProviderMessage::content_type() const {
    return message_content_type;
  }

  const std::unordered_map<std::string, std::string>& ProviderMessage::metadata() const {
    return message_metadata;
  }

  ////////////////////////////////////
  // MessageProviderServer
  ////////////////////////////////////

  namespace {
    /**
     * Pulls the body out of a V4 message contents document, which looks like
     * {"content": ..., "contentType": "...", "encoded": false|"base64"}.
     */
    void read_message_contents(const json& contents, std::string& body, std::string& content_type) {
      if (!contents.is_object()) {
        return;
      }
      if (contents.contains("contentType") && contents["contentType"].is_string()) {
        content_type = contents["contentType"].get<std::string>();
      }
      if (!contents.contains("content")) {
        return;
      }

      const json& content = contents["content"];
      body = content.is_string() ? content.get<std::string>() : content.dump();

      bool encoded = contents.contains("encoded") &&
        contents["encoded"].is_string() && contents["encoded"].get<std::string>() == "base64";
      if (encoded && content.is_string()) {
        body = base64_decode(body);
      }
    }
  }

  struct MessageProviderServer::Impl {
    std::string path;
    http::Server server;
    std::unordered_map<std::string, MessageHandler> handlers;
    MessageHandler default_handler;
    std::mutex lock;

    explicit Impl(std::string endpoint_path) : path(std::move(endpoint_path)) {
      if (path.empty() || path.front() != '/') {
        path = "/" + path;
      }
    }

    void handle(const http::Request& request, http::Response& response) {
      if (request.path != path) {
        response.status = 404;
        response.body = R"({"error":"Not found"})";
        return;
      }

      json body = json::parse(request.body, nullptr, false);
      if (body.is_discarded() || !body.is_object()) {
        response.status = 400;
        response.body = R"({"error":"Request body was not a JSON object"})";
        return;
      }

      MessageRequest message_request;
      if (body.contains("description") && body["description"].is_string()) {
        message_request.description = body["description"].get<std::string>();
      }
      if (body.contains("providerStates") && body["providerStates"].is_array()) {
        for (const auto& state : body["providerStates"]) {
          if (!state.is_object()) {
            continue;
          }
          MessageProviderState provider_state;
          if (state.contains("name") && state["name"].is_string()) {
            provider_state.name = state["name"].get<std::string>();
          }
          if (state.contains("params") && state["params"].is_object()) {
            provider_state.params = ProviderStateParams(state["params"].dump());
          }
          message_request.provider_states.push_back(std::move(provider_state));
        }
      }
      // Only synchronous messages carry the consumer's request message
      if (body.contains("request")) {
        message_request.synchronous = true;
        message_request.request_json = body["request"].dump();
        if (body["request"].is_object() && body["request"].contains("contents")) {
          read_message_contents(body["request"]["contents"], message_request.request_body,
            message_request.request_content_type);
        }
      }

      MessageHandler handler = lookup_handler(lock, handlers, default_handler, message_request.description);

      if (!handler) {
        response.status = 500;
        response.body = json({{"error",
          "No handler registered for message '" + message_request.description + "'"}}).dump();
        return;
      }

      try {
        ProviderMessage contents = handler(message_request);
        response.status = 200;
        response.body = contents.body();
        response.content_type = contents.content_type();

        if (!contents.metadata().empty()) {
          json metadata = json::object();
          for (const auto& entry : contents.metadata()) {
            json value = json::parse(entry.second, nullptr, false);
            metadata[entry.first] = value.is_discarded() ? json(entry.second) : value;
          }
          // The verifier expects the metadata as base64 encoded JSON in this header
          response.headers["Pact-Message-Metadata"] = base64_encode(metadata.dump());
        }
      } catch (const std::exception& e) {
        response.status = 500;
        response.content_type = "application/json";
        response.body = json({{"error", std::string("Message handler failed: ") + e.what()}}).dump();
      } catch (...) {
        response.status = 500;
        response.content_type = "application/json";
        response.body = json({{"error", "Message handler failed with an unknown exception"}}).dump();
      }
    }
  };

  MessageProviderServer::MessageProviderServer(std::string path)
    : impl(std::make_unique<Impl>(std::move(path))) {
    impl->server.set_handler([this](const http::Request& request, http::Response& response) {
      impl->handle(request, response);
    });
  }

  MessageProviderServer::~MessageProviderServer() {
    stop();
  }

  void MessageProviderServer::add_message_handler(const std::string& description, MessageHandler handler) {
    std::lock_guard<std::mutex> guard(impl->lock);
    impl->handlers[description] = std::move(handler);
  }

  void MessageProviderServer::set_default_handler(MessageHandler handler) {
    std::lock_guard<std::mutex> guard(impl->lock);
    impl->default_handler = std::move(handler);
  }

  bool MessageProviderServer::start(uint16_t port, const std::string& host) {
    return impl->server.start(port, host);
  }

  void MessageProviderServer::stop() {
    impl->server.stop();
  }

  bool MessageProviderServer::is_running() const {
    return impl->server.is_running();
  }

  uint16_t MessageProviderServer::get_port() const {
    return impl->server.get_port();
  }

  const std::string& MessageProviderServer::get_path() const {
    return impl->path;
  }

  std::string MessageProviderServer::get_url() const {
    return impl->server.get_url() + impl->path;
  }

  ////////////////////////////////////
  // VerificationResult
  ////////////////////////////////////

  bool VerificationResult::is_ok() const {
    return code == 0;
  }

  int VerificationResult::result_code() const {
    return code;
  }

  const std::string& VerificationResult::output() const {
    return verifier_output;
  }

  const std::string& VerificationResult::logs() const {
    return verifier_logs;
  }

  const std::string& VerificationResult::json() const {
    return verifier_json;
  }

  void VerificationResult::display() const {
    std::cout << verifier_output << std::endl;
  }

  ////////////////////////////////////
  // Verifier
  ////////////////////////////////////

  struct Verifier::Impl {
    VerifierHandle* handle = nullptr;
    std::unique_ptr<ProviderStateServer> state_server;
    bool has_state_handlers = false;
    bool state_change_teardown = true;
    bool state_change_body = true;
    uint16_t state_change_port = 0;
    std::unique_ptr<MessageProviderServer> message_server;
    bool has_message_handlers = false;
    uint16_t message_transport_port = 0;
    std::string message_endpoint_path = "/__pact/message";
    std::string provider_host = "127.0.0.1";
    bool strip_ansi = false;

    /**
     * The hosted state change and message endpoints have to be reachable at the
     * provider host, but only an IPv4 literal can be bound directly.
     */
    std::string bind_host() const {
      int parts = 1;
      for (char c : provider_host) {
        if (c == '.') {
          parts++;
        } else if (!std::isdigit(static_cast<unsigned char>(c))) {
          return "127.0.0.1";
        }
      }
      return parts == 4 ? provider_host : "127.0.0.1";
    }

    ProviderStateServer& states() {
      if (!state_server) {
        state_server = std::make_unique<ProviderStateServer>();
      }
      has_state_handlers = true;
      return *state_server;
    }

    MessageProviderServer& messages() {
      if (!message_server) {
        message_server = std::make_unique<MessageProviderServer>(message_endpoint_path);
      }
      has_message_handlers = true;
      return *message_server;
    }
  };

  Verifier::Verifier() : impl(std::make_unique<Impl>()) {
    // Reported to the broker as the application that ran the verification
    init();
    impl->handle = pactffi_verifier_new_for_application("pact-cpp", PACT_CPP_VERIFIER_VERSION);
    if (impl->handle == nullptr) {
      throw std::runtime_error("Failed to create the Pact verifier");
    }
  }

  Verifier::~Verifier() {
    if (impl) {
      if (impl->state_server) {
        impl->state_server->stop();
      }
      if (impl->message_server) {
        impl->message_server->stop();
      }
      if (impl->handle != nullptr) {
        pactffi_verifier_shutdown(impl->handle);
        impl->handle = nullptr;
      }
    }
  }

  Verifier::Verifier(Verifier&& other) noexcept : impl(std::move(other.impl)) {}

  Verifier& Verifier::operator=(Verifier&& other) noexcept {
    if (this != &other) {
      if (impl && impl->handle != nullptr) {
        pactffi_verifier_shutdown(impl->handle);
      }
      impl = std::move(other.impl);
    }
    return *this;
  }

  Verifier& Verifier::set_provider_info(const std::string& name, const std::string& scheme,
    const std::string& host, uint16_t port, const std::string& path) {
    if (!host.empty()) {
      impl->provider_host = host;
    }
    pactffi_verifier_set_provider_info(impl->handle, or_null(name), or_null(scheme),
      or_null(host), port, or_null(path));
    return *this;
  }

  Verifier& Verifier::add_provider_transport(const std::string& protocol, uint16_t port,
    const std::string& path, const std::string& scheme) {
    pactffi_verifier_add_provider_transport(impl->handle, or_null(protocol), port,
      or_null(path), or_null(scheme));
    return *this;
  }

  Verifier& Verifier::add_custom_header(const std::string& name, const std::string& value) {
    pactffi_verifier_add_custom_header(impl->handle, name.c_str(), value.c_str());
    return *this;
  }

  Verifier& Verifier::set_follow_redirects(bool follow) {
    pactffi_verifier_set_follow_redirects(impl->handle, follow ? 1 : 0);
    return *this;
  }

  Verifier& Verifier::set_verification_options(bool disable_ssl_verification, unsigned long request_timeout) {
    pactffi_verifier_set_verification_options(impl->handle, disable_ssl_verification ? 1 : 0, request_timeout);
    return *this;
  }

  Verifier& Verifier::set_provider_state_url(const std::string& url, bool teardown, bool body) {
    pactffi_verifier_set_provider_state(impl->handle, or_null(url), teardown ? 1 : 0, body ? 1 : 0);
    return *this;
  }

  Verifier& Verifier::add_state_handler(const std::string& state, StateHandler handler) {
    impl->states().add_state_handler(state, std::move(handler));
    return *this;
  }

  Verifier& Verifier::add_state_handler_with_values(const std::string& state, StateHandlerWithValues handler) {
    impl->states().add_state_handler_with_values(state, std::move(handler));
    return *this;
  }

  Verifier& Verifier::set_default_state_handler(StateHandler handler) {
    impl->states().set_default_handler(std::move(handler));
    return *this;
  }

  Verifier& Verifier::set_default_state_handler_with_values(StateHandlerWithValues handler) {
    impl->states().set_default_handler_with_values(std::move(handler));
    return *this;
  }

  Verifier& Verifier::set_state_change_port(uint16_t port) {
    impl->state_change_port = port;
    return *this;
  }

  Verifier& Verifier::set_state_change_teardown(bool teardown) {
    impl->state_change_teardown = teardown;
    return *this;
  }

  Verifier& Verifier::set_state_change_body(bool body) {
    impl->state_change_body = body;
    return *this;
  }

  Verifier& Verifier::add_message_handler(const std::string& description, MessageHandler handler) {
    impl->messages().add_message_handler(description, std::move(handler));
    return *this;
  }

  Verifier& Verifier::set_default_message_handler(MessageHandler handler) {
    impl->messages().set_default_handler(std::move(handler));
    return *this;
  }

  Verifier& Verifier::set_message_transport_port(uint16_t port) {
    impl->message_transport_port = port;
    return *this;
  }

  Verifier& Verifier::set_message_endpoint_path(const std::string& path) {
    if (impl->message_server) {
      throw std::logic_error("The message endpoint path must be set before adding message handlers");
    }
    impl->message_endpoint_path = path;
    return *this;
  }

  Verifier& Verifier::set_filter_info(const std::string& filter_description,
    const std::string& filter_state, bool filter_no_state) {
    pactffi_verifier_set_filter_info(impl->handle, or_null(filter_description),
      or_null(filter_state), filter_no_state ? 1 : 0);
    return *this;
  }

  Verifier& Verifier::set_consumer_filters(const std::vector<std::string>& consumers) {
    auto values = as_c_array(consumers);
    pactffi_verifier_set_consumer_filters(impl->handle, values.data(),
      static_cast<unsigned short>(values.size()));
    return *this;
  }

  Verifier& Verifier::add_file_source(const std::string& file) {
    pactffi_verifier_add_file_source(impl->handle, file.c_str());
    return *this;
  }

  Verifier& Verifier::add_directory_source(const std::string& directory) {
    pactffi_verifier_add_directory_source(impl->handle, directory.c_str());
    return *this;
  }

  Verifier& Verifier::add_url_source(const std::string& url, const std::string& username,
    const std::string& password, const std::string& token) {
    pactffi_verifier_url_source(impl->handle, url.c_str(), or_null(username),
      or_null(password), or_null(token));
    return *this;
  }

  Verifier& Verifier::add_broker_source(const std::string& url, const std::string& username,
    const std::string& password, const std::string& token) {
    pactffi_verifier_broker_source(impl->handle, url.c_str(), or_null(username),
      or_null(password), or_null(token));
    return *this;
  }

  bool Verifier::add_broker_source_with_selectors(const BrokerSelectorOptions& options) {
    auto provider_tags = as_c_array(options.provider_tags);
    auto selectors = as_c_array(options.consumer_version_selectors);
    auto consumer_tags = as_c_array(options.consumer_version_tags);

    return pactffi_verifier_broker_source_with_selectors(
      impl->handle,
      options.url.c_str(),
      or_null(options.username),
      or_null(options.password),
      or_null(options.token),
      options.enable_pending ? 1 : 0,
      or_null(options.include_wip_pacts_since),
      provider_tags.data(),
      static_cast<unsigned short>(provider_tags.size()),
      or_null(options.provider_branch),
      selectors.data(),
      static_cast<unsigned short>(selectors.size()),
      consumer_tags.data(),
      static_cast<unsigned short>(consumer_tags.size())) == 0;
  }

  Verifier& Verifier::set_publish_options(const PublishOptions& options) {
    auto provider_tags = as_c_array(options.provider_tags);
    pactffi_verifier_set_publish_options(
      impl->handle,
      options.provider_version.c_str(),
      or_null(options.build_url),
      provider_tags.data(),
      static_cast<unsigned short>(provider_tags.size()),
      or_null(options.provider_branch));
    return *this;
  }

  Verifier& Verifier::set_coloured_output(bool coloured) {
    pactffi_verifier_set_coloured_output(impl->handle, coloured ? 1 : 0);
    return *this;
  }

  Verifier& Verifier::set_no_pacts_is_error(bool is_error) {
    pactffi_verifier_set_no_pacts_is_error(impl->handle, is_error ? 1 : 0);
    return *this;
  }

  Verifier& Verifier::set_strip_ansi_from_output(bool strip) {
    impl->strip_ansi = strip;
    return *this;
  }

  VerificationResult Verifier::execute() {
    bool started_state_server = false;
    if (impl->has_state_handlers && !impl->state_server->is_running()) {
      if (!impl->state_server->start(impl->state_change_port, impl->bind_host())) {
        throw std::runtime_error("Failed to start the provider state change server");
      }
      started_state_server = true;
      pactffi_verifier_set_provider_state(impl->handle, impl->state_server->get_url().c_str(),
        impl->state_change_teardown ? 1 : 0, impl->state_change_body ? 1 : 0);
    }

    bool started_message_server = false;
    if (impl->has_message_handlers && !impl->message_server->is_running()) {
      if (!impl->message_server->start(impl->message_transport_port, impl->bind_host())) {
        throw std::runtime_error("Failed to start the message provider server");
      }
      started_message_server = true;
      pactffi_verifier_add_provider_transport(impl->handle, "message",
        impl->message_server->get_port(), impl->message_server->get_path().c_str(), "http");
    }

    VerificationResult result;
    result.code = pactffi_verifier_execute(impl->handle);
    result.verifier_output = take_string(pactffi_verifier_output(impl->handle, impl->strip_ansi ? 1 : 0));
    result.verifier_logs = take_string(pactffi_verifier_logs(impl->handle));
    result.verifier_json = take_string(pactffi_verifier_json(impl->handle));

    if (started_state_server) {
      impl->state_server->stop();
    }
    if (started_message_server) {
      impl->message_server->stop();
    }

    return result;
  }

  std::string Verifier::logs_for_provider(const std::string& provider_name) {
    return take_string(pactffi_verifier_logs_for_provider(provider_name.c_str()));
  }

  VerifierHandle* Verifier::get_handle() const {
    return impl->handle;
  }
}
