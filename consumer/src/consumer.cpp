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
}
