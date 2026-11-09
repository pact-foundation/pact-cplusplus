#include <consumer.h>
#include <verifier.h>

int main() {
    pact_consumer::init();
    auto pact = pact_consumer::Pact("ExampleConsumer", "ExampleProvider");

    pact_verifier::Verifier verifier;
    verifier.set_provider_info("ExampleProvider");
    return 0;
}
