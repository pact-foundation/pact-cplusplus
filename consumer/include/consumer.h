#pragma once

#include <string>
#include <unordered_map>
#include <vector>
extern "C" {
#include <pact.h>
}
#include <filesystem>
#include <optional>
#include <functional>
#include "matchers.h"

namespace pact_consumer {

  /**
   * Initilise the pact test library
   */
  void init();

  class Interaction;

  /**
   * Mock server handle to the mock server started for the test
   */
  class MockServerHandle {
    public:
      MockServerHandle(PactHandle);
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
       * Adds a test state with a message to the result
       */
      void add_state(TestResultState state, std::string message, std::string ex);

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
      std::optional<std::string> ex;
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
       * Creates a new iteraction with a defined provider state and parameters
       */
      Interaction given(const char* provider_state, const std::unordered_map<std::string, std::string>& parameters) const;

      /**
       * Creates a new interaction with the provided description.
       */
      Interaction uponReceiving(const char* description) const;

      /**
       * Starts a mock server for this pact, and then passes it to the callback. The callback
       * needs to return a boolean value to indicate of the test was successful.
       */
      PactTestResult run_test(std::function<bool(const MockServerHandle*)> callback) const;

      PactHandle pact;

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
     * Adds a provider state with parameters
     */
    Interaction given(const char* provider_state, const std::unordered_map<std::string, std::string>& parameters) const;

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
    Interaction withQuery(const std::unordered_map<std::string, std::vector<std::string>>& query) const;

    /**
     * Sets the headers for the request
     */
    Interaction withHeaders(const std::unordered_map<std::string, std::vector<std::string>>& headers) const;
    
    /**
     * Sets the body for the request to the string contents.
     */
    Interaction withBody(const std::string& body, const std::string& content_type) const;

    /**
     * Sets the body for the request using the provided body template.
     */
    Interaction withJsonBody(pact_consumer::matchers::IMatcher::Ptr body) const;

    /**
     * Sets the body for the request using the example file and content type. Note that this will attempt to load the
     * entire example file in memory. Use small files for your testing.
     */
    Interaction withBinaryFile(const std::string& content_type, const std::filesystem::path& example_file) const;

    /**
     * Sets the body for the request as a MIME multipart body using the example file and content type. Note that this will attempt to load the
     * entire example file in memory. Use small files for your testing.
     */
    Interaction withMultipartFileUpload(const std::string& part_name, const std::string& content_type, const std::filesystem::path& example_file) const;

    /**
     * Sets the status code for the response
     */
    Interaction willRespondWith(size_t status) const;

    /**
     * Sets the headers for the response
     */
    Interaction withResponseHeaders(const std::unordered_map<std::string, std::vector<std::string>>& headers) const;

    /**
     * Sets the body for the response to the string contents.
     */
    Interaction withResponseBody(const std::string& body, const std::string& content_type) const;

    /**
     * Sets the body for the request using the provided body template
     */
    Interaction withResponseJsonBody(pact_consumer::matchers::IMatcher::Ptr body) const;

    /**
     * Sets the body for the response using the example file and content type. Note that this will attempt to load the
     * entire example file in memory. Use small files for your testing. 
     */
    Interaction withResponseBinaryFile(const std::string& content_type, const std::filesystem::path& example_file) const;

    /**
     * Sets the body for the response as a MIME multipart body using the example file and content type. Note that this will attempt to load the
     * entire example file in memory. Use small files for your testing. 
     */
    Interaction withResponseMultipartFileUpload(const std::string& part_name, const std::string& content_type, const std::filesystem::path& example_file) const;

    InteractionHandle interaction;

    private:
      std::string description;
      const Pact* pact;
  };
}
