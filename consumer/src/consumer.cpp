#include "consumer.h"

using namespace std;

namespace pact_consumer
{
  Pact::Pact(string consumer, string provider) {
    this->consumer = consumer;
    this->provider = provider;
  };
}
