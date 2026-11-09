#include <iostream>

#include "route_guide_service.h"
#include "verifier.h"

int main() {
  pact_verifier::init();
  RouteGuideServer server(ROUTE_GUIDE_DATABASE);
  if (!server.Start()) {
    std::cerr << "Unable to start the RouteGuide provider" << std::endl;
    return 1;
  }

  pact_verifier::Verifier verifier;
  verifier
      .set_provider_info("route-guide-provider")
      .add_provider_transport("grpc", server.port())
      .add_file_source(ROUTE_GUIDE_PACT_FILE)
      .add_state_handler("the Patriots Path feature exists", [](const auto&) {});

  auto result = verifier.execute();
  result.display();
  return result.is_ok() ? 0 : 1;
}