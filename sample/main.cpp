#include <consumer.h>

#include <iostream>

int main() {
  pact_consumer::init();

  auto provider = pact_consumer::Pact("SampleConsumer", "SampleProvider");
  provider.pact_directory = "pacts";

  provider
    .given("a project exists")
    .uponReceiving("a request for the project")
    .withRequest("GET", "/projects/1001")
    .withResponseHeaders({{"Content-Type", {"application/json"}}})
    .willRespondWith(200)
    .withResponseJsonBody(pact_consumer::matchers::Object({
      {"id", pact_consumer::matchers::Integer(1001)},
      {"name", pact_consumer::matchers::Like("Example project")}
    }));

  auto result = provider.run_test([](const auto* mock_server) {
    std::cout << "Mock server started at " << mock_server->get_url() << std::endl;
    std::cout << "No request is made, so the configured interaction will mismatch." << std::endl;
    return true;
  });

  if (result.is_ok()) {
    std::cerr << "Expected an unmet-request mismatch, but the test passed." << std::endl;
    return 1;
  }

  std::cerr << "Test failed as expected because no request was made." << std::endl;
  return 1;
}