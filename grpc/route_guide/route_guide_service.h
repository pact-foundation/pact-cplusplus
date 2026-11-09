#pragma once

#include <grpcpp/server.h>

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "route_guide.grpc.pb.h"

class RouteGuideService final : public routeguide::RouteGuide::Service {
 public:
  explicit RouteGuideService(const std::string& db);

  grpc::Status GetFeature(grpc::ServerContext* context,
                          const routeguide::Point* point,
                          routeguide::Feature* feature) override;
  grpc::Status ListFeatures(grpc::ServerContext* context,
                            const routeguide::Rectangle* rectangle,
                            grpc::ServerWriter<routeguide::Feature>* writer) override;
  grpc::Status RecordRoute(grpc::ServerContext* context,
                           grpc::ServerReader<routeguide::Point>* reader,
                           routeguide::RouteSummary* summary) override;
  grpc::Status RouteChat(
      grpc::ServerContext* context,
      grpc::ServerReaderWriter<routeguide::RouteNote, routeguide::RouteNote>* stream) override;

 private:
  std::vector<routeguide::Feature> feature_list_;
  std::mutex mutex_;
  std::vector<routeguide::RouteNote> received_notes_;
};

class RouteGuideServer {
 public:
  explicit RouteGuideServer(const std::string& db);
  ~RouteGuideServer();

  bool Start(const std::string& address = "127.0.0.1:0");
  void Wait();
  void Shutdown();
  int port() const;

 private:
  RouteGuideService service_;
  std::unique_ptr<grpc::Server> server_;
  int port_ = 0;
};