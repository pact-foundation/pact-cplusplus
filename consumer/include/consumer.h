#pragma once

#include <string>
#include <pact_mock_server_ffi.h>

namespace pact_consumer
{
  class Interaction;

  class Pact {
    public:
      Pact(const char* consumer, const char* provider);
      Interaction given(const char* provider_state) const;
      Interaction uponReceiving(const char* description) const;

      pact_mock_server_ffi::PactHandle pact;

    private:
      std::string consumer;
      std::string provider;
  };

  class Interaction {
    public:
    Interaction(const Pact* parent, const char *description);
    Interaction given(const char* provider_state) const;
    Interaction uponReceiving(const char* description) const;

    pact_mock_server_ffi::InteractionHandle interaction;

    private:
      std::string description;
      const Pact* pact;
  };
}
