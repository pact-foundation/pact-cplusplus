#include "route_guide_client.h"

#include <grpcpp/client_context.h>

RouteGuideClient::RouteGuideClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(routeguide::RouteGuide::NewStub(std::move(channel))) {}

grpc::Status RouteGuideClient::GetFeature(const routeguide::Point& point,
                                          routeguide::Feature* feature) const {
  grpc::ClientContext context;
  return stub_->GetFeature(&context, point, feature);
}