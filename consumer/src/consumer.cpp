#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <spdlog/spdlog.h>
#include <spdlog/details/os.h>
#include <boost/algorithm/string.hpp>
#include "consumer.h"

using namespace pact_mock_server_ffi;

namespace pact_consumer {
  void init() {
    pact_mock_server_ffi::init("LOG_LEVEL");
    const char *env_p = getenv("LOG_LEVEL");
    if (env_p != nullptr) {
      std::string level_str = env_p;
      std::string level_str_lower = boost::to_lower_copy(level_str);
      auto level = spdlog::level::from_str(level_str_lower);
      spdlog::details::registry::instance().set_level(level);
    }
  }

  ////////////////////////////////////
  // Pact Class
  ////////////////////////////////////

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

  PactTestResult Pact::run_test(bool (*callback)(MockServerHandle&)) const {
    MockServerHandle mockServer(this->pact);
    PactTestResult result;

    if (mockServer.started_ok()) {
      try {
        bool callback_result = callback(mockServer);
        bool mock_server_result = mock_server_matched(mockServer.get_port());
        if (callback_result && mock_server_result) {
          auto write_result = write_pact_file(mockServer.get_port(), this->pact_directory.data());
          switch (write_result) {
            case 1:
              result.add_state(TestResultState::PactFileError, "A general panic was caught");
              break;
            case 2:
              result.add_state(TestResultState::PactFileError, "The pact file was not able to be written");
              break;
            case 3:
              result.add_state(TestResultState::PactFileError, "A mock server with the provided port was not found");
              break;
          }
        }
      } catch(const std::exception& e) {
        result.add_state(TestResultState::UserCodeFailed, e.what());
      } catch (...) {
        result.add_state(TestResultState::UserCodeFailed);
      }

      if (!mock_server_matched(mockServer.get_port())) {
        std::string mismatches = mock_server_mismatches(mockServer.get_port());
        result.add_state(TestResultState::Mismatches, mismatches);
      }
    } else {
      result.add_state(TestResultState::MockServerFailed);
    }

    if (!result.is_ok()) {
      result.display_errors();
    }

    return result;
  }

  ////////////////////////////////////
  // Interaction Class
  ////////////////////////////////////

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

  Interaction Interaction::withQuery(std::unordered_map<std::string, std::vector<std::string>> query) const {
    for (auto q : query) {
      for (auto it = q.second.begin(); it != q.second.end(); it++) {
        pact_mock_server_ffi::with_query_parameter(this->interaction, q.first.data(), it - q.second.begin(), it->data());
      }
    }
    return *this;
  }
  
  Interaction Interaction::withHeaders(std::unordered_map<std::string, std::vector<std::string>> headers) const {
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

  Interaction Interaction::withResponseHeaders(std::unordered_map<std::string, std::vector<std::string>> headers) const {
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

  ////////////////////////////////////
  // Mock Server Class
  ////////////////////////////////////

  MockServerHandle::MockServerHandle(pact_mock_server_ffi::PactHandle pact) {
    this->port = pact_mock_server_ffi::create_mock_server_for_pact(pact, "127.0.0.1:0");
  }

  MockServerHandle::~MockServerHandle() {
    pact_mock_server_ffi::cleanup_mock_server(this->port);
  }

  bool MockServerHandle::started_ok() const {
    return this->port > 0;
  }

  std::string MockServerHandle::get_url() const {
    std::ostringstream out;
    out << "http://127.0.0.1:" << this->port << "/";
    return out.str();
  }

  int32_t MockServerHandle::get_port() const {
    return this->port;
  }

  ////////////////////////////////////
  // Pact Test Result Class
  ////////////////////////////////////

  PactTestResult::PactTestResult() {
    this->messages.resize(TestResultState::MockServerFailed + 1);
  }

  void PactTestResult::add_state(TestResultState state) {
    this->status |= state;
  }

  void PactTestResult::add_state(TestResultState state, std::string message) {
    this->status |= state;
    this->messages[state] = message;
  }

  bool PactTestResult::is_ok() const {
    return this->status == 0;
  }

  void PactTestResult::display_errors() {
    if (this->status != 0) {
      spdlog::error("The test failed for the following reasons:");
      if (this->status & TestResultState::Mismatches) {
        if (this->messages[TestResultState::Mismatches].empty()) {
          spdlog::error("\t* Not all the requests matched");
        } else {
          spdlog::error("\t* The following mismatches occurred:");
          auto mismatches_str = this->messages[TestResultState::Mismatches];
          auto j = json::parse(mismatches_str);
          int i = 0;
          for (json::iterator it = j.begin(); it != j.end(); ++it, i++) {
            auto mismatch = *it;
            std::string type = mismatch["type"];
            if (type == "request-not-found") {
              spdlog::error("\t\t{}) Unexpected request received: {} {}", i, mismatch["method"], mismatch["path"]);
            } else if (type == "missing-request") {
              spdlog::error("\t\t{}) Expected request was not received: {} {}", i, mismatch["method"], mismatch["path"]);
            } else if (type == "request-mismatch") {
              spdlog::error("\t\t{}) Mismatched request was received: {} {}", i, mismatch["method"], mismatch["path"]);
              auto mismatches = mismatch["mismatches"];
              for (json::iterator it = mismatches.begin(); it != mismatches.end(); ++it) {
                auto mismatch_details = *it;
                std::string mismatch_type = mismatch_details["type"];
                std::string expected = mismatch_details["expected"];
                std::string actual = mismatch_details["actual"];
                if (mismatch_type == "MethodMismatch") {
                  spdlog::error("\t\t\tMethod: Expected {} but was {}", expected, actual);
                } else if (mismatch_type == "PathMismatch") {
                  spdlog::error("\t\t\tPath: {}", mismatch_details["mismatch"]);
                } else if (mismatch_type == "StatusMismatch") {
                  spdlog::error("\t\t\tStatus: Expected {} but was {}", expected, actual);
                } else if (mismatch_type == "QueryMismatch") {
                  spdlog::error("\t\t\tQuery Parameter: {}", mismatch_details["mismatch"]);
                } else if (mismatch_type == "HeaderMismatch") {
                  spdlog::error("\t\t\tHeader: {}", mismatch_details["mismatch"]);
                } else if (mismatch_type == "BodyTypeMismatch") {
                  spdlog::error("\t\t\tBody Type: Expected {} but was {}", expected, actual);
                } else if (mismatch_type == "BodyMismatch") {
                  spdlog::error("\t\t\tBody: {}", mismatch_details["mismatch"]);
                }
              }
            }
          }
        }
      }
      if (this->status & TestResultState::UserCodeFailed) {
        auto message = this->messages[TestResultState::UserCodeFailed];
        if (message.empty()) {
          spdlog::error("\t* Test callback failed with an exception");
        } else {
          spdlog::error("\t* Test callback failed with an exception: {}", message);
        }
      }
      if (this->status & TestResultState::PactFileError) {
        auto message = this->messages[TestResultState::PactFileError];
        if (message.empty()) {
          spdlog::error("\t* Failed to write the Pact file");
        } else {
          spdlog::error("\t* Failed to write the Pact file: {}", message);
        }
      }
      if (this->status & TestResultState::MockServerFailed) {
        auto message = this->messages[TestResultState::MockServerFailed];
        if (message.empty()) {
          spdlog::error("\t* Mock server failed to start");
        } else {
          spdlog::error("\t* Mock server failed to start: {}", message);
        }
      }
    }
  }
}
