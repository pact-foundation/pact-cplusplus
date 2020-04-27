#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <pact_mock_server_ffi.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace pact_consumer {

  /**
   * Initilise the pact test library
   */
  void init();

  class Interaction;

  /**
   * Builder class to help construct JSON bodies 
   */
  class PactJsonBuilder {
    public:
      PactJsonBuilder(const PactJsonBuilder* parent);

      /**
       * Attribute that is an array where each item in the array must match the constructed template
       * @param name Attribute name
       * @param examples Number of examples to generate (defaults to 1) 
       * @param callback Callback that gets invoked to define the template
       */
      PactJsonBuilder& eachLike(std::string name, unsigned int examples, void (*callback)(PactJsonBuilder*));

      /**
       * Attribute that is an array where each item in the array must match the constructed template. Will only generate one example.
       * @param name Attribute name
       * @param callback Callback that gets invoked to define the template
       */
      PactJsonBuilder& eachLike(std::string name, void (*callback)(PactJsonBuilder*));

      /**
      * Attribute that is an array that has to have at least one element and each element must match the given template
      * @param name Attribute name
      * @param examples Number of examples to generate (defaults to 1) 
      * @param callback Callback that gets invoked to define the template
      */ 
      PactJsonBuilder& atLeastOneLike(std::string name, unsigned int examples, void (*callback)(PactJsonBuilder*));

      /**
      * Attribute that is an array that has to have at least one element and each element must match the given template. Will only generate one example.
      * @param name Attribute name
      * @param callback Callback that gets invoked to define the template
      */ 
      PactJsonBuilder& atLeastOneLike(std::string name, void (*callback)(PactJsonBuilder*));

      /**
       * Attribute whose value must be an integer (must be a number and have no decimal places)
       * @param name Attribute name
       * @param example Example value. If omitted a random value will be generated.
       */
      PactJsonBuilder& integer(std::string name, int example);

      /**
       * Attribute whose value must be an integer (must be a number and have no decimal places). A random value will be 
       * generated for any examples.
       * @param name Attribute name
       */
      PactJsonBuilder& integer(std::string name);

      /**
       * Attribute whose value must be a string.
       * @param name Attribute name
       * @param example Example value.
       */
      PactJsonBuilder& string(std::string name, std::string example);

      /**
      * String value that must match the provided datetime format string.
      * @param name Attribute name
      * @param format Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)
      * @param example Example value to use. If omitted a value using the current system date and time will be generated.
      */
      PactJsonBuilder& datetime(std::string name, std::string format, std::string example);

      /**
      * String value that must match the provided datetime format string. A random value will be 
      * @param name Attribute name
      * generated for any examples.
      * @param format Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)
      */
      PactJsonBuilder& datetime(std::string name, std::string format);

      /**
      * Value must be a boolean
      * @param name Attribute name
      * @param b Boolean example value
      */
      PactJsonBuilder& boolean(std::string name, bool b);

      json get_json() { return obj; }

    private:
      const PactJsonBuilder* parent;
      json obj;
  };

  /**
   * Mock server handle to the mock server started for the test
   */
  class MockServerHandle {
    public:
      MockServerHandle(pact_mock_server_ffi::PactHandle);
      ~MockServerHandle();

      /**
       * Indicates the mock server was started ok
       */
      bool started_ok() const;

      /**
       * The URL to the mock server
       */
      std::string get_url() const;

      /**
       * The port the mock server is running on
       */
      int32_t get_port() const;

    private:
      int32_t port;
  };

  /**
   * Type of Pact test result
   */
  enum TestResultState { 
    Mismatches        = (1u << 0),
    UserCodeFailed    = (1u << 1),
    PactFileError     = (1u << 2),
    MockServerFailed  = (1u << 3)
  };

  /**
   * Result of the Pact test run
   */
  class PactTestResult {
    public:
      PactTestResult();

      /**
       * Adds a test state to the result
       */
      void add_state(TestResultState state);

      /**
       * Adds a test state with a message to the result
       */
      void add_state(TestResultState state, std::string message);

      /**
       * If there are no mismatches and the user code did not fail
       */
      bool is_ok() const;

      /**
       * Logs all errors out
       */
      void display_errors();

    private:
      unsigned int status = 0;
      std::vector<std::string> messages;
  };

  /**
   * Class that defines a Pact between a consumer and provider
   */
  class Pact {
    public:
      /**
       * Constructs the Pact given the consumer and provider names
       */
      Pact(const char* consumer, const char* provider);

      /**
       * Creates a new iteraction with a defined provider state
       */
      Interaction given(const char* provider_state) const;

      /**
       * Creates a new interaction with the provided description.
       */
      Interaction uponReceiving(const char* description) const;

      /**
       * Starts a mock server for this pact, and then passes it to the callback. The callback
       * needs to return a boolean value to indicate of the test was successful.
       */
      PactTestResult run_test(bool (*callback)(MockServerHandle*)) const;

      pact_mock_server_ffi::PactHandle pact;

      /**
       * Directory to write pact files to
       */
      std::string pact_directory;

    private:
      std::string consumer;
      std::string provider;
  };

  /**
   * Class that defines an interaction between a consumer and provider
   */
  class Interaction {
    public:
    Interaction(const Pact* parent, const char *description);

    /**
     * Adds the provider state to the interaction
     */
    Interaction given(const char* provider_state) const;
    /**
     * Sets the description for the interaction
     */
    Interaction uponReceiving(const char* description) const;
    /**
     * Sets the method and path for the request
     */
    Interaction withRequest(const char* method, const char* path) const;
    /**
     * Sets the query parameters for the request
     */
    Interaction withQuery(std::unordered_map<std::string, std::vector<std::string>> query) const;
    /**
     * Sets the headers for the request
     */
    Interaction withHeaders(std::unordered_map<std::string, std::vector<std::string>> headers) const;
    /**
     * Sets the body for the request using the callback. The callback will be invoked 
     * with a builder to construct the body.
     */
    Interaction withJsonBody(void (*callback)(PactJsonBuilder*)) const;
    /**
     * Sets the status code for the response
     */
    Interaction willRespondWith(size_t status) const;
    /**
     * Sets the headers for the response
     */
    Interaction withResponseHeaders(std::unordered_map<std::string, std::vector<std::string>> headers) const;
    /**
     * Sets the body for the request using the callback. The callback will be invoked 
     * with a builder to construct the body.
     */
    Interaction withResponseJsonBody(void (*callback)(PactJsonBuilder*)) const;

    pact_mock_server_ffi::InteractionHandle interaction;

    private:
      std::string description;
      const Pact* pact;
  };
}
