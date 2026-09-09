#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

extern "C" {
#include <pact.h>
}

#include "http_server.h"

namespace pact_verifier {

  /**
   * Initialise the Pact library. Reads the log level from the `LOG_LEVEL`
   * environment variable.
   */
  void init();

  /**
   * Log levels supported by the Pact core.
   */
  enum class LogLevel {
    Off,
    Error,
    Warn,
    Info,
    Debug,
    Trace
  };

  /** Sends Pact core logs to standard output. */
  bool log_to_stdout(LogLevel level = LogLevel::Info);
  /** Sends Pact core logs to standard error. */
  bool log_to_stderr(LogLevel level = LogLevel::Info);
  /** Sends Pact core logs to a file. */
  bool log_to_file(const std::string& file_name, LogLevel level = LogLevel::Info);
  /**
   * Buffers Pact core logs in memory. Required if you want to read
   * VerificationResult::logs after a verification run.
   */
  bool log_to_buffer(LogLevel level = LogLevel::Debug);

  /**
   * Whether the verifier is asking for a provider state to be set up before an
   * interaction, or torn down afterwards.
   */
  enum class StateAction {
    Setup,
    Teardown
  };

  /**
   * Parameters supplied with a provider state, as defined by the consumer with
   * `given(state, params)`.
   *
   * Values are exposed as strings. JSON strings are unquoted; any other JSON
   * value (numbers, booleans, objects, arrays) is returned as its raw JSON text.
   * Use json() if you need the untouched document.
   */
  class ProviderStateParams {
    public:
      ProviderStateParams();
      explicit ProviderStateParams(const std::string& json);

      /** True if a parameter with this name was supplied */
      bool contains(const std::string& name) const;

      /** The parameter value, or an empty optional if it was not supplied */
      std::optional<std::string> get(const std::string& name) const;

      /** The parameter value, or the supplied default if it was not supplied */
      std::string get_or(const std::string& name, const std::string& default_value) const;

      /** The parameter parsed as an integer, or an empty optional if missing or not numeric */
      std::optional<long long> get_int(const std::string& name) const;

      /** The parameter parsed as a double, or an empty optional if missing or not numeric */
      std::optional<double> get_double(const std::string& name) const;

      /** The parameter parsed as a boolean, or an empty optional if missing or not a boolean */
      std::optional<bool> get_bool(const std::string& name) const;

      /** All parameters keyed by name */
      const std::unordered_map<std::string, std::string>& values() const;

      /** The raw JSON document the parameters were parsed from */
      const std::string& json() const;

    private:
      std::string params_json;
      std::unordered_map<std::string, std::string> params;
  };

  /**
   * A provider state change request received from the verifier.
   */
  struct ProviderStateRequest {
    /** The provider state description, e.g. "the user exists" */
    std::string state;
    /** Whether this is a setup or teardown call */
    StateAction action = StateAction::Setup;
    /** Any parameters the consumer attached to the state */
    ProviderStateParams params;
  };

  /** Callback that puts the provider into the required state. */
  using StateHandler = std::function<void(const ProviderStateRequest&)>;

  /**
   * Callback that puts the provider into the required state and returns a JSON
   * object of values. Those values are made available to the verifier for use by
   * generators (`fromProviderState`). Return an empty string for no values.
   */
  using StateHandlerWithValues = std::function<std::string(const ProviderStateRequest&)>;

  /**
   * Hosts the HTTP endpoint the Pact verifier calls to set up and tear down
   * provider states.
   *
   * The verifier does not call your state handlers directly, it makes an HTTP
   * request for each provider state, so a server has to be running for the
   * duration of the verification. Verifier::add_state_handler starts and stops
   * one of these for you; use this class directly only if you want to host the
   * endpoint yourself (for example inside your real application under test).
   */
  class ProviderStateServer {
    public:
      /**
       * @param path Path the state change endpoint is served on
       */
      explicit ProviderStateServer(std::string path = "/__pact/provider-state");
      ~ProviderStateServer();

      ProviderStateServer(const ProviderStateServer&) = delete;
      ProviderStateServer& operator=(const ProviderStateServer&) = delete;

      /** Registers a handler for a single provider state */
      void add_state_handler(const std::string& state, StateHandler handler);

      /** Registers a handler for a single provider state that returns generator values */
      void add_state_handler_with_values(const std::string& state, StateHandlerWithValues handler);

      /** Registers a handler invoked for any state without a specific handler */
      void set_default_handler(StateHandler handler);

      /** Registers a value returning handler invoked for any state without a specific handler */
      void set_default_handler_with_values(StateHandlerWithValues handler);

      /**
       * If true (the default), a state change request for a state with no registered
       * handler and no default handler fails with a 500. Set to false to ignore them.
       */
      void set_unknown_state_is_error(bool is_error);

      /**
       * Starts the server. Pass 0 to let the OS pick a free port.
       * Returns false if the port could not be bound.
       */
      bool start(uint16_t port = 0, const std::string& host = "127.0.0.1");

      /** Stops the server. Safe to call more than once */
      void stop();

      /** True while the server is running */
      bool is_running() const;

      /** The port the server is bound to */
      uint16_t get_port() const;

      /** The full URL of the state change endpoint, to hand to the verifier */
      std::string get_url() const;

    private:
      struct Impl;
      std::unique_ptr<Impl> impl;
  };

  /**
   * Options for fetching pacts from a Pact Broker or PactFlow using consumer
   * version selectors.
   *
   * @see https://docs.pact.io/pact_broker/advanced_topics/consumer_version_selectors
   */
  struct BrokerSelectorOptions {
    /** Base URL of the broker, e.g. "https://my-org.pactflow.io" */
    std::string url;
    /** Basic auth username. Leave empty to omit */
    std::string username;
    /** Basic auth password. Leave empty to omit */
    std::string password;
    /** Bearer token. Leave empty to omit. Mutually exclusive with username/password */
    std::string token;
    /** Include pacts that are in pending state, so failures do not break the build */
    bool enable_pending = false;
    /** ISO date (YYYY-MM-DD). Include work in progress pacts created since this date */
    std::string include_wip_pacts_since;
    /** Tags to apply to the provider version */
    std::vector<std::string> provider_tags;
    /** Branch the provider is being verified from */
    std::string provider_branch;
    /** Consumer version selectors, each as a JSON object string */
    std::vector<std::string> consumer_version_selectors;
    /** Legacy consumer version tags. Prefer consumer_version_selectors */
    std::vector<std::string> consumer_version_tags;
  };

  /**
   * Options for publishing verification results back to the broker.
   */
  struct PublishOptions {
    /** Version of the provider being verified, e.g. a git SHA */
    std::string provider_version;
    /** URL of the CI build that ran the verification. Optional */
    std::string build_url;
    /** Tags to apply to the provider version. Optional */
    std::vector<std::string> provider_tags;
    /** Branch the provider is being verified from. Optional */
    std::string provider_branch;
  };

  /**
   * The outcome of a verification run.
   */
  class VerificationResult {
    public:
      /** True if every interaction was verified successfully */
      bool is_ok() const;

      /** Raw exit code from the verifier. Zero means success */
      int result_code() const;

      /** Human readable verifier output, as printed by the Pact CLI verifier */
      const std::string& output() const;

      /** Verifier logs. Only populated if log_to_buffer() was called first */
      const std::string& logs() const;

      /** Machine readable verification result as a JSON document */
      const std::string& json() const;

      /** Writes the output to standard output */
      void display() const;

    private:
      friend class Verifier;

      int code = -1;
      std::string verifier_output;
      std::string verifier_logs;
      std::string verifier_json;
  };

  /**
   * Verifies that a provider honours the contracts published by its consumers.
   *
   * Every option exposed by the Pact FFI verifier is available here. Setters
   * return a reference to the verifier so calls can be chained.
   *
   * @code
   * pact_verifier::Verifier verifier;
   * verifier
   *   .set_provider_info("my-provider", "http", "localhost", 8080, "/")
   *   .add_file_source("./pacts/my-consumer-my-provider.json")
   *   .add_state_handler("the user exists", [](const auto& request) {
   *     seed_user(request.params.get_int("id").value_or(0));
   *   });
   *
   * auto result = verifier.execute();
   * result.display();
   * @endcode
   */
  class Verifier {
    public:
      /**
       * Creates a verifier. The calling application is reported to the Pact Broker as
       * "pact-cpp" at this library's version, which the broker records against the
       * verification results.
       */
      Verifier();

      ~Verifier();

      Verifier(const Verifier&) = delete;
      Verifier& operator=(const Verifier&) = delete;
      Verifier(Verifier&& other) noexcept;
      Verifier& operator=(Verifier&& other) noexcept;

      //
      // Provider under test
      //

      /**
       * Details of the running provider the interactions are replayed against.
       * Empty strings and a zero port fall back to the Pact defaults
       * (http://localhost:8080/).
       */
      Verifier& set_provider_info(const std::string& name, const std::string& scheme = "http",
        const std::string& host = "localhost", uint16_t port = 8080, const std::string& path = "/");

      /**
       * Adds an additional transport for the provider, for message pacts or
       * plugin based protocols. Use protocol "message" for non-plugin async messages.
       */
      Verifier& add_provider_transport(const std::string& protocol, uint16_t port,
        const std::string& path = "", const std::string& scheme = "");

      /** Adds a header to every request the verifier makes to the provider */
      Verifier& add_custom_header(const std::string& name, const std::string& value);

      /** Whether the verifier should follow redirects returned by the provider. Default true */
      Verifier& set_follow_redirects(bool follow);

      /**
       * @param disable_ssl_verification Skip TLS certificate checks, for self signed certs
       * @param request_timeout Timeout in milliseconds for requests to the provider
       */
      Verifier& set_verification_options(bool disable_ssl_verification, unsigned long request_timeout);

      //
      // Provider states
      //

      /**
       * Tells the verifier to POST (or GET) state change requests to this URL.
       * Use this when your application hosts the state change endpoint itself.
       *
       * @param teardown Also send a teardown request after each interaction
       * @param body Send the state data as a JSON body rather than query parameters
       */
      Verifier& set_provider_state_url(const std::string& url, bool teardown = true, bool body = true);

      /**
       * Registers a handler for a provider state. The verifier hosts a state change
       * endpoint on your behalf for the duration of execute().
       */
      Verifier& add_state_handler(const std::string& state, StateHandler handler);

      /**
       * Registers a handler for a provider state that returns a JSON object of values
       * for the verifier to feed into `fromProviderState` generators.
       */
      Verifier& add_state_handler_with_values(const std::string& state, StateHandlerWithValues handler);

      /** Registers a handler used for any state without a specific handler */
      Verifier& set_default_state_handler(StateHandler handler);

      /** Registers a value returning handler used for any state without a specific handler */
      Verifier& set_default_state_handler_with_values(StateHandlerWithValues handler);

      /**
       * Port for the hosted state change endpoint. Defaults to 0, meaning the OS
       * picks a free port.
       */
      Verifier& set_state_change_port(uint16_t port);

      /** Whether the hosted endpoint should also receive teardown calls. Default true */
      Verifier& set_state_change_teardown(bool teardown);

      /** Whether state change data is sent as a body rather than query params. Default true */
      Verifier& set_state_change_body(bool body);

      //
      // Filtering
      //

      /**
       * Restricts which interactions are verified.
       *
       * @param filter_description Regular expression matched against the interaction description
       * @param filter_state Regular expression matched against the provider state
       * @param filter_no_state Only verify interactions that have no provider state
       */
      Verifier& set_filter_info(const std::string& filter_description = "",
        const std::string& filter_state = "", bool filter_no_state = false);

      /** Only verify pacts from these consumers */
      Verifier& set_consumer_filters(const std::vector<std::string>& consumers);

      //
      // Pact sources
      //

      /** Verifies a single pact file */
      Verifier& add_file_source(const std::string& file);

      /** Verifies every pact in a directory that matches the provider name */
      Verifier& add_directory_source(const std::string& directory);

      /** Fetches a single pact from a URL */
      Verifier& add_url_source(const std::string& url, const std::string& username = "",
        const std::string& password = "", const std::string& token = "");

      /** Fetches all the latest pacts for the provider from a broker */
      Verifier& add_broker_source(const std::string& url, const std::string& username = "",
        const std::string& password = "", const std::string& token = "");

      /**
       * Fetches pacts from a broker using consumer version selectors, pending pacts
       * and work in progress pacts.
       *
       * Returns false if any of the consumer version selectors were not valid JSON.
       */
      bool add_broker_source_with_selectors(const BrokerSelectorOptions& options);

      //
      // Results
      //

      /** Publishes verification results back to the broker */
      Verifier& set_publish_options(const PublishOptions& options);

      /** Enables or disables ANSI colour codes in the verifier output. Default true */
      Verifier& set_coloured_output(bool coloured);

      /** Whether finding no pacts to verify is a failure. Default true */
      Verifier& set_no_pacts_is_error(bool is_error);

      /** Strips ANSI escape codes from VerificationResult::output. Default false */
      Verifier& set_strip_ansi_from_output(bool strip);

      /**
       * Runs the verification. If any state handlers were registered, the state change
       * endpoint is started before and stopped after the run.
       */
      VerificationResult execute();

      /** Fetches buffered logs for a provider by name, outside of a verification run */
      static std::string logs_for_provider(const std::string& provider_name);

      /** The underlying FFI handle, for options not yet wrapped by this class */
      VerifierHandle* get_handle() const;

    private:
      struct Impl;
      std::unique_ptr<Impl> impl;
  };
}
