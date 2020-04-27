#include <iostream>
#include <consumer.h>

int main() {
    pact_consumer::init();
    auto provider = pact_consumer::Pact("PackageConsumer", "PackageProvider");
}
