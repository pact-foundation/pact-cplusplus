#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <iostream>
#include <string>

#include "consumer.h"
#include "route_guide_client.h"

namespace {

constexpr int kLatitude = 407838351;
constexpr int kLongitude = -746143763;
constexpr const char* kFeatureName = "Patriots Path, Mendham, NJ 07945, USA";

std::string EscapeJson(std::string value) {
  std::string escaped;
  for (char character : value) {
    if (character == '\\' || character == '"') {
      escaped.push_back('\\');
    }
    escaped.push_back(character);
  }
  return escaped;
}

class PluginCleanup {
 public:
  explicit PluginCleanup(const pact_consumer::Pact& pact) : pact_(pact) {}
  ~PluginCleanup() { pact_.cleanupPlugins(); }

 private:
  const pact_consumer::Pact& pact_;
};

int RunConsumerTest() {
  pact_consumer::init();
  pact_consumer::Pact provider("route-guide-consumer", "route-guide-provider");
  provider.pact_directory = ROUTE_GUIDE_PACT_DIR;
  provider.withSpecification(PactSpecification_V4);
  if (!provider.usingPlugin("protobuf", "0.8.0")) {
    std::cerr << "Unable to load pact-protobuf-plugin 0.8.0" << std::endl;
    return 1;
  }
  PluginCleanup cleanup(provider);

  const std::string interaction =
      "{\"pact:proto\":\"" + EscapeJson(ROUTE_GUIDE_PROTO_FILE) +
      "\",\"pact:proto-service\":\"RouteGuide/GetFeature\"," 
      "\"pact:content-type\":\"application/protobuf\"," 
      "\"request\":{\"latitude\":\"matching(number, 407838351)\"," 
      "\"longitude\":\"matching(number, -746143763)\"},"
      "\"response\":{\"name\":\"matching(type, '" +
      std::string(kFeatureName) +
      "')\",\"location\":{\"latitude\":\"matching(number, 407838351)\"," 
      "\"longitude\":\"matching(number, -746143763)\"}}}";

  provider
      .newSyncMessage("get a feature by its location")
      .given("the Patriots Path feature exists")
      .withPluginContents("application/protobuf", interaction);

  auto result = provider.run_test("grpc", [](const auto* mock_server) {
    RouteGuideClient client(grpc::CreateChannel(
        "127.0.0.1:" + std::to_string(mock_server->get_port()),
        grpc::InsecureChannelCredentials()));
    routeguide::Point point;
    point.set_latitude(kLatitude);
    point.set_longitude(kLongitude);
    routeguide::Feature feature;
    auto status = client.GetFeature(point, &feature);
    return status.ok() && feature.name() == kFeatureName &&
           feature.location().latitude() == kLatitude &&
           feature.location().longitude() == kLongitude;
  });

  return result.is_ok() ? 0 : 1;
}

}  // namespace

int main() { return RunConsumerTest(); }