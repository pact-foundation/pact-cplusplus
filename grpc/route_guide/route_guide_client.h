#pragma once

#include <grpcpp/channel.h>

#include <memory>

#include "route_guide.grpc.pb.h"

class RouteGuideClient {
 public:
  explicit RouteGuideClient(std::shared_ptr<grpc::Channel> channel);

  grpc::Status GetFeature(const routeguide::Point& point,
                          routeguide::Feature* feature) const;

 private:
  std::unique_ptr<routeguide::RouteGuide::Stub> stub_;
};