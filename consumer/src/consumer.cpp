#include <iostream>
#include <stdlib.h>
#include "consumer.h"

using namespace std;
using namespace pact_mock_server_ffi;

namespace pact_consumer
{
  Pact::Pact(const char* consumer_name, const char* provider_name) {
    this->pact =  pact_mock_server_ffi::new_pact(consumer_name, provider_name);
    this->consumer = consumer_name;
    this->provider = provider_name;
  }

  Interaction Pact::uponReceiving(const char* description) const {
    return Interaction(this, description);
  }

  Interaction Pact::given(const char* provider_state) const {
    return Interaction(this, "__new_interaction__").given(provider_state);
  }

  Interaction::Interaction(const Pact* parent, const char* description) {
    this->pact =  parent;
    this->description = description;
    this->interaction = pact_mock_server_ffi::new_interaction(parent->pact, description);
  }

  Interaction Interaction::uponReceiving(const char* description) const {
    pact_mock_server_ffi::upon_receiving(this->interaction, description);
    return *this;
  }

  Interaction Interaction::given(const char* provider_state) const {
    pact_mock_server_ffi::given(this->interaction, provider_state);
    return *this;
  }

  Interaction Interaction::withRequest(const char* method, const char* path) const {
    pact_mock_server_ffi::with_request(this->interaction, method, path);
    return *this;
  }

  Interaction Interaction::withQuery(unordered_map<string, vector<string>> query) const {
    for (auto q : query) {
      for (auto it = q.second.begin(); it != q.second.end(); it++) {
        pact_mock_server_ffi::with_query_parameter(this->interaction, q.first.data(), it - q.second.begin(), it->data());
      }
    }
    return *this;
  }
  
  Interaction Interaction::withHeaders(unordered_map<string, vector<string>> headers) const {
    for (auto h : headers) {
      for (auto it = h.second.begin(); it != h.second.end(); it++) {
        pact_mock_server_ffi::with_header(this->interaction, pact_mock_server_ffi::InteractionPart::Request, h.first.data(), it - h.second.begin(), it->data());
      }
    }
    return *this;
  }

  Interaction Interaction::withJsonBody(void (*callback)(PactJsonBuilder&)) const {
    PactJsonBuilder body(NULL);
    callback(body);
    pact_mock_server_ffi::with_body(this->interaction, pact_mock_server_ffi::InteractionPart::Request, "application/json;charset=UTF-8", 
      body.get_json().dump().data());
    return *this;
  }

  Interaction Interaction::willRespondWith(size_t status) const {
    pact_mock_server_ffi::response_status(this->interaction, status);
    return *this;
  }

  Interaction Interaction::withResponseHeaders(unordered_map<string, vector<string>> headers) const {
    for (auto h : headers) {
      for (auto it = h.second.begin(); it != h.second.end(); it++) {
        pact_mock_server_ffi::with_header(this->interaction, pact_mock_server_ffi::InteractionPart::Response, h.first.data(), it - h.second.begin(), it->data());
      }
    }
    return *this;
  }

  Interaction Interaction::withResponseJsonBody(void (*callback)(PactJsonBuilder&)) const {
    PactJsonBuilder body(NULL);
    callback(body);
    pact_mock_server_ffi::with_body(this->interaction, pact_mock_server_ffi::InteractionPart::Response, "application/json;charset=UTF-8", 
      body.get_json().dump().data());
    return *this;
  }
}
