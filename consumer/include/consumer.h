#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <pact_mock_server_ffi.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

namespace pact_consumer
{
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
      PactJsonBuilder eachLike(string name, unsigned int examples, void (*callback)(PactJsonBuilder&));
      /**
       * Attribute that is an array where each item in the array must match the constructed template
       * @param name Attribute name
       * @param callback Callback that gets invoked to define the template
       */
      PactJsonBuilder eachLike(string name, void (*callback)(PactJsonBuilder&));

      json get_json() { return obj; }

    private:
      const PactJsonBuilder* parent;
      json obj;
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

      pact_mock_server_ffi::PactHandle pact;

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
    Interaction withQuery(unordered_map<string, vector<string>> query) const;
    /**
     * Sets the headers for the request
     */
    Interaction withHeaders(unordered_map<string, vector<string>> headers) const;
    /**
     * Sets the body for the request using the callback. The callback will be invoked 
     * with a builder to construct the body.
     */
    Interaction withJsonBody(void (*callback)(PactJsonBuilder&)) const;
    /**
     * Sets the status code for the response
     */
    Interaction willRespondWith(size_t status) const;
    /**
     * Sets the headers for the response
     */
    Interaction withResponseHeaders(unordered_map<string, vector<string>> headers) const;
    /**
     * Sets the body for the request using the callback. The callback will be invoked 
     * with a builder to construct the body.
     */
    Interaction withResponseJsonBody(void (*callback)(PactJsonBuilder&)) const;

    pact_mock_server_ffi::InteractionHandle interaction;

    private:
      std::string description;
      const Pact* pact;
  };
}
