#include "route_guide_service.h"

#include <grpcpp/server_builder.h>

#include <algorithm>
#include <chrono>
#include <cmath>

#include "helper.h"

namespace {

float ConvertToRadians(float number) { return number * 3.1415926F / 180; }

float GetDistance(const routeguide::Point& start,
                  const routeguide::Point& end) {
  constexpr float kCoordinateFactor = 10000000.0F;
  float latitude1 = start.latitude() / kCoordinateFactor;
  float latitude2 = end.latitude() / kCoordinateFactor;
  float longitude1 = start.longitude() / kCoordinateFactor;
  float longitude2 = end.longitude() / kCoordinateFactor;
  float latitudeRadians1 = ConvertToRadians(latitude1);
  float latitudeRadians2 = ConvertToRadians(latitude2);
  float latitudeDelta = ConvertToRadians(latitude2 - latitude1);
  float longitudeDelta = ConvertToRadians(longitude2 - longitude1);
  float value = std::pow(std::sin(latitudeDelta / 2), 2) +
                std::cos(latitudeRadians1) * std::cos(latitudeRadians2) *
                    std::pow(std::sin(longitudeDelta / 2), 2);
  return 6371000 * 2 * std::atan2(std::sqrt(value), std::sqrt(1 - value));
}

std::string GetFeatureName(
    const routeguide::Point& point,
    const std::vector<routeguide::Feature>& feature_list) {
  for (const auto& feature : feature_list) {
    if (feature.location().latitude() == point.latitude() &&
        feature.location().longitude() == point.longitude()) {
      return feature.name();
    }
  }
  return "";
}

}  // namespace

RouteGuideService::RouteGuideService(const std::string& db) {
  routeguide::ParseDb(db, &feature_list_);
}

grpc::Status RouteGuideService::GetFeature(grpc::ServerContext*,
                                           const routeguide::Point* point,
                                           routeguide::Feature* feature) {
  feature->set_name(GetFeatureName(*point, feature_list_));
  feature->mutable_location()->CopyFrom(*point);
  return grpc::Status::OK;
}

grpc::Status RouteGuideService::ListFeatures(
    grpc::ServerContext*, const routeguide::Rectangle* rectangle,
    grpc::ServerWriter<routeguide::Feature>* writer) {
  const auto& low = rectangle->lo();
  const auto& high = rectangle->hi();
  long left = std::min(low.longitude(), high.longitude());
  long right = std::max(low.longitude(), high.longitude());
  long top = std::max(low.latitude(), high.latitude());
  long bottom = std::min(low.latitude(), high.latitude());
  for (const auto& feature : feature_list_) {
    if (feature.location().longitude() >= left &&
        feature.location().longitude() <= right &&
        feature.location().latitude() >= bottom &&
        feature.location().latitude() <= top) {
      writer->Write(feature);
    }
  }
  return grpc::Status::OK;
}

grpc::Status RouteGuideService::RecordRoute(
    grpc::ServerContext*, grpc::ServerReader<routeguide::Point>* reader,
    routeguide::RouteSummary* summary) {
  routeguide::Point point;
  routeguide::Point previous;
  int point_count = 0;
  int feature_count = 0;
  float distance = 0.0F;
  auto start = std::chrono::system_clock::now();
  while (reader->Read(&point)) {
    ++point_count;
    if (!GetFeatureName(point, feature_list_).empty()) {
      ++feature_count;
    }
    if (point_count != 1) {
      distance += GetDistance(previous, point);
    }
    previous = point;
  }
  summary->set_point_count(point_count);
  summary->set_feature_count(feature_count);
  summary->set_distance(static_cast<long>(distance));
  summary->set_elapsed_time(std::chrono::duration_cast<std::chrono::seconds>(
                                std::chrono::system_clock::now() - start)
                                .count());
  return grpc::Status::OK;
}

grpc::Status RouteGuideService::RouteChat(
    grpc::ServerContext*,
    grpc::ServerReaderWriter<routeguide::RouteNote, routeguide::RouteNote>* stream) {
  routeguide::RouteNote note;
  while (stream->Read(&note)) {
    std::unique_lock<std::mutex> lock(mutex_);
    for (const auto& received_note : received_notes_) {
      if (received_note.location().latitude() == note.location().latitude() &&
          received_note.location().longitude() == note.location().longitude()) {
        stream->Write(received_note);
      }
    }
    received_notes_.push_back(note);
  }
  return grpc::Status::OK;
}

RouteGuideServer::RouteGuideServer(const std::string& db) : service_(db) {}

RouteGuideServer::~RouteGuideServer() { Shutdown(); }

bool RouteGuideServer::Start(const std::string& address) {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(address, grpc::InsecureServerCredentials(), &port_);
  builder.RegisterService(&service_);
  server_ = builder.BuildAndStart();
  return server_ != nullptr;
}

void RouteGuideServer::Wait() {
  if (server_) {
    server_->Wait();
  }
}

void RouteGuideServer::Shutdown() {
  if (server_) {
    server_->Shutdown();
    server_.reset();
  }
}

int RouteGuideServer::port() const { return port_; }