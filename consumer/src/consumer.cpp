#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <spdlog/spdlog.h>
#include <spdlog/details/os.h>
#include <boost/algorithm/string.hpp>
#include "consumer.h"

using namespace std;
using namespace pact_mock_server_ffi;

namespace pact_consumer {
  void init() {
    pact_mock_server_ffi::init("LOG_LEVEL");
    const char *env_p = getenv("LOG_LEVEL");
    if (env_p != nullptr) {
      string level_str = env_p;
      string level_str_lower = boost::to_lower_copy(level_str);
      auto level = spdlog::level::from_str(level_str_lower);
      spdlog::details::registry::instance().set_level(level);
    }
  }

  Pact::Pact(const char* consumer_name, const char* provider_name) {
    this->pact =  pact_mock_server_ffi::new_pact(consumer_name, provider_name);
    this->consumer = consumer_name;
    this->provider = provider_name;
  }

  Interaction Pact::uponReceiving(const char* description) const {
    return Interaction(this, description);
  }

  Interaction Pact::given(const char* provider_state) const {
    return Interaction(this, "__new_interaction__").given(provider_state);
  }

  Interaction::Interaction(const Pact* parent, const char* description) {
    this->pact =  parent;
    this->description = description;
    this->interaction = pact_mock_server_ffi::new_interaction(parent->pact, description);
  }

  Interaction Interaction::uponReceiving(const char* description) const {
    pact_mock_server_ffi::upon_receiving(this->interaction, description);
    return *this;
  }

  Interaction Interaction::given(const char* provider_state) const {
    pact_mock_server_ffi::given(this->interaction, provider_state);
    return *this;
  }

  Interaction Interaction::withRequest(const char* method, const char* path) const {
    pact_mock_server_ffi::with_request(this->interaction, method, path);
    return *this;
  }

  Interaction Interaction::withQuery(unordered_map<string, vector<string>> query) const {
    for (auto q : query) {
      for (auto it = q.second.begin(); it != q.second.end(); it++) {
        pact_mock_server_ffi::with_query_parameter(this->interaction, q.first.data(), it - q.second.begin(), it->data());
      }
    }
    return *this;
  }
  
  Interaction Interaction::withHeaders(unordered_map<string, vector<string>> headers) const {
    for (auto h : headers) {
      for (auto it = h.second.begin(); it != h.second.end(); it++) {
        pact_mock_server_ffi::with_header(this->interaction, pact_mock_server_ffi::InteractionPart::Request, h.first.data(), it - h.second.begin(), it->data());
      }
    }
    return *this;
  }

  Interaction Interaction::withJsonBody(void (*callback)(PactJsonBuilder&)) const {
    PactJsonBuilder body(nullptr);
    callback(body);
    pact_mock_server_ffi::with_body(this->interaction, pact_mock_server_ffi::InteractionPart::Request, "application/json;charset=UTF-8", 
      body.get_json().dump().data());
    return *this;
  }

  Interaction Interaction::willRespondWith(size_t status) const {
    pact_mock_server_ffi::response_status(this->interaction, status);
    return *this;
  }

  Interaction Interaction::withResponseHeaders(unordered_map<string, vector<string>> headers) const {
    for (auto h : headers) {
      for (auto it = h.second.begin(); it != h.second.end(); it++) {
        pact_mock_server_ffi::with_header(this->interaction, pact_mock_server_ffi::InteractionPart::Response, h.first.data(), it - h.second.begin(), it->data());
      }
    }
    return *this;
  }

  Interaction Interaction::withResponseJsonBody(void (*callback)(PactJsonBuilder&)) const {
    PactJsonBuilder body(nullptr);
    callback(body);
    pact_mock_server_ffi::with_body(this->interaction, pact_mock_server_ffi::InteractionPart::Response, "application/json;charset=UTF-8", 
      body.get_json().dump().data());
    return *this;
  }

  PactTestResult Pact::run_test(void (*callback)(MockServerHandle&)) const {
    MockServerHandle mockServer(this->pact);
    PactTestResult result;
    if (mockServer.started_ok()) {
      try {
        callback(mockServer);
      } catch(const std::exception& e) {
        spdlog::error("Test run failed with an exception: {}", e.what());
        result.add_state(TestResultState::UserCodeFailed);
      } catch (...) {
        spdlog::error("Test run failed with an exception");
        result.add_state(TestResultState::UserCodeFailed);
      }
    } else {
      spdlog::error("Failed to start the mock server");
      result.add_state(TestResultState::MockServerFailed);
    }
    return result;
  }

  MockServerHandle::MockServerHandle(pact_mock_server_ffi::PactHandle pact) {
    this->port = pact_mock_server_ffi::create_mock_server_for_pact(pact, "127.0.0.1:0");
  }

  MockServerHandle::~MockServerHandle() {
    pact_mock_server_ffi::cleanup_mock_server(this->port);
  }

  bool MockServerHandle::started_ok() const {
    return this->port > 0;
  }

  string MockServerHandle::get_url() const {
    std::ostringstream out;
    out << "http://127.0.0.1:" << this->port << "/";
    return out.str();
  }

  void PactTestResult::add_state(TestResultState state) {
    this->status |= state;
  }

  bool PactTestResult::is_ok() const {
    return this->status == 0;
  }
}
