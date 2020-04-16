#pragma once

#include <string>

namespace pact_consumer
{
  class Pact {
    public:
      Pact(std::string consumer, std::string provider);

    private:
      std::string consumer;
      std::string provider;
  };
}
