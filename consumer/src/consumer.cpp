#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include "consumer.h"

using namespace pact_mock_server_ffi;

namespace pact_consumer {
  void init() {
    pact_mock_server_ffi::init("LOG_LEVEL");
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

  Interaction Pact::given(const char* provider_state, std::unordered_map<std::string, std::string> parameters) const {
    return Interaction(this, "__new_interaction__").given(provider_state, parameters);
  }

  PactTestResult Pact::run_test(bool (*callback)(MockServerHandle*)) const {
    MockServerHandle mockServer(this->pact);
    PactTestResult result;

    if (mockServer.started_ok()) {
      try {
        bool callback_result = callback(&mockServer);
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
    if (this->interaction.interaction == 0) {
      throw std::string("Could not create a new interaction with description ") + description;
    }
  }

  Interaction Interaction::uponReceiving(const char* description) const {
    pact_mock_server_ffi::upon_receiving(this->interaction, description);
    return *this;
  }

  Interaction Interaction::given(const char* provider_state) const {
    pact_mock_server_ffi::given(this->interaction, provider_state);
    return *this;
  }

  Interaction Interaction::given(const char* provider_state, std::unordered_map<std::string, std::string> parameters) const {
    for (auto& p : parameters) {
      pact_mock_server_ffi::given_with_param(this->interaction, provider_state, p.first.data(), p.second.data());
    }
    return *this;
  }

  Interaction Interaction::withRequest(const char* method, const char* path) const {
    pact_mock_server_ffi::with_request(this->interaction, method, path);
    return *this;
  }

  Interaction Interaction::withQuery(std::unordered_map<std::string, std::vector<std::string>> query) const {
    for (auto& q : query) {
      for (auto it = q.second.begin(); it != q.second.end(); it++) {
        pact_mock_server_ffi::with_query_parameter(this->interaction, q.first.data(), it - q.second.begin(), it->data());
      }
    }
    return *this;
  }
  
  Interaction Interaction::withHeaders(std::unordered_map<std::string, std::vector<std::string>> headers) const {
    for (auto& h : headers) {
      for (auto it = h.second.begin(); it != h.second.end(); it++) {
        pact_mock_server_ffi::with_header(this->interaction, pact_mock_server_ffi::InteractionPart::Request, h.first.data(), it - h.second.begin(), it->data());
      }
    }
    return *this;
  }

  Interaction Interaction::withJsonBody(void (*callback)(PactJsonBuilder*)) const {
    PactJsonBuilder body(nullptr);
    callback(&body);
    pact_mock_server_ffi::with_body(this->interaction, pact_mock_server_ffi::InteractionPart::Request, "application/json;charset=UTF-8", 
      body.get_json().dump().data());
    return *this;
  }

  Interaction Interaction::withBinaryFile(std::string content_type, std::filesystem::path example_file) const {
    std::ifstream file (example_file, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
      pact_mock_server_ffi::with_binary_file(this->interaction, pact_mock_server_ffi::InteractionPart::Request, content_type.data(), 
        buffer.data(), size);
      return *this;
    } else {
      throw std::string("Could not read file contents: ") + example_file.string();
    }
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

  Interaction Interaction::withResponseJsonBody(void (*callback)(PactJsonBuilder*)) const {
    PactJsonBuilder body(nullptr);
    callback(&body);
    pact_mock_server_ffi::with_body(this->interaction, pact_mock_server_ffi::InteractionPart::Response, "application/json;charset=UTF-8", 
      body.get_json().dump().data());
    return *this;
  }

  Interaction Interaction::withResponseBinaryFile(std::string content_type, std::filesystem::path example_file) const {
    std::ifstream file (example_file, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
      pact_mock_server_ffi::with_binary_file(this->interaction, pact_mock_server_ffi::InteractionPart::Response, content_type.data(), 
        buffer.data(), size);
      return *this;
    } else {
      throw std::string("Could not read file contents: ") + example_file.string();
    }
  }

  ////////////////////////////////////
  // Mock Server Class
  ////////////////////////////////////

  MockServerHandle::MockServerHandle(pact_mock_server_ffi::PactHandle pact) {
    this->port = pact_mock_server_ffi::create_mock_server_for_pact(pact, "127.0.0.1:0", false);
  }

  MockServerHandle::~MockServerHandle() {
    pact_mock_server_ffi::cleanup_mock_server(this->port);
  }

  bool MockServerHandle::started_ok() const {
    return this->port > 0;
  }

  std::string MockServerHandle::get_url() const {
    std::ostringstream out;
    out << "http://127.0.0.1:" << this->port;
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
      std::cout << "The test failed for the following reasons:\n";
      if (this->status & TestResultState::Mismatches) {
        if (this->messages[TestResultState::Mismatches].empty()) {
          std::cout << "\t* Not all the requests matched\n";
        } else {
          std::cout << "\t* The following mismatches occurred:\n";
          auto mismatches_str = this->messages[TestResultState::Mismatches];
          auto j = json::parse(mismatches_str);
          int i = 0;
          for (json::iterator it = j.begin(); it != j.end(); ++it, i++) {
            auto mismatch = *it;
            std::string type = mismatch["type"];
            if (type == "request-not-found") {
              std::cout << "\t\t" << i << ") Unexpected request received: " << mismatch["method"] << " " << mismatch["path"] << "\n";
            } else if (type == "missing-request") {
              std::cout << "\t\t" << i << ") Expected request was not received: " << mismatch["method"] << " " << mismatch["path"] << "\n";
            } else if (type == "request-mismatch") {
              std::cout << "\t\t" << i << ") Mismatched request was received: " << mismatch["method"] << " " << mismatch["path"] << "\n";
              auto mismatches = mismatch["mismatches"];
              for (json::iterator it = mismatches.begin(); it != mismatches.end(); ++it) {
                auto mismatch_details = *it;
                std::string mismatch_type = mismatch_details["type"];
                std::string expected = mismatch_details["expected"];
                std::string actual = mismatch_details["actual"];
                if (mismatch_type == "MethodMismatch") {
                  std::cout << "\t\t\tMethod: Expected " << expected << " but was " << actual << "\n";
                } else if (mismatch_type == "PathMismatch") {
                  std::cout << "\t\t\tPath: " << mismatch_details["mismatch"] << "\n";
                } else if (mismatch_type == "StatusMismatch") {
                  std::cout << "\t\t\tStatus: Expected " << expected << " but was " << actual << "\n";
                } else if (mismatch_type == "QueryMismatch") {
                  std::cout << "\t\t\tQuery Parameter: " << mismatch_details["mismatch"] << "\n";
                } else if (mismatch_type == "HeaderMismatch") {
                  std::cout << "\t\t\tHeader: " << mismatch_details["mismatch"] << "\n";
                } else if (mismatch_type == "BodyTypeMismatch") {
                  std::cout << "\t\t\tBody Type: Expected " << expected << " but was " << actual << "\n";
                } else if (mismatch_type == "BodyMismatch") {
                  std::cout << "\t\t\tBody: " << mismatch_details["mismatch"] << "\n";
                }
              }
            }
          }
        }
      }
      if (this->status & TestResultState::UserCodeFailed) {
        auto message = this->messages[TestResultState::UserCodeFailed];
        if (message.empty()) {
          std::cout << "\t* Test callback failed with an exception\n";
        } else {
          std::cout << "\t* Test callback failed with an exception: " << message << "\n";
        }
      }
      if (this->status & TestResultState::PactFileError) {
        auto message = this->messages[TestResultState::PactFileError];
        if (message.empty()) {
          std::cout << "\t* Failed to write the Pact file\n";
        } else {
          std::cout << "\t* Failed to write the Pact file: " << message << "\n";
        }
      }
      if (this->status & TestResultState::MockServerFailed) {
        auto message = this->messages[TestResultState::MockServerFailed];
        if (message.empty()) {
          std::cout << "\t* Mock server failed to start\n";
        } else {
          std::cout << "\t* Mock server failed to start: " << message << "\n";
        }
      }
    }
  }
}
