#include <verifier.h>

int main() {
    pact_verifier::init();
    pact_verifier::Verifier verifier;
    verifier.set_provider_info("PackageProvider");
}
