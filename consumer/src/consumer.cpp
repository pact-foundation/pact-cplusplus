#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include "consumer.h"
#include <boost/exception/diagnostic_information.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace pact_consumer {
  void init() {
    pactffi_init("LOG_LEVEL");
  }

  ////////////////////////////////////
  // Pact Class
  ////////////////////////////////////

  Pact::Pact(const char* consumer_name, const char* provider_name) {
    this->pact =  pactffi_new_pact(consumer_name, provider_name);
    this->consumer = consumer_name;
    this->provider = provider_name;
  }

  Interaction Pact::uponReceiving(const char* description) const {
    return Interaction(this, description);
  }

  Interaction Pact::given(const char* provider_state) const {
    return Interaction(this, "__new_interaction__").given(provider_state);
  }

  Interaction Pact::given(const char* provider_state, const std::unordered_map<std::string, std::string>& parameters) const {
    return Interaction(this, "__new_interaction__").given(provider_state, parameters);
  }

  PactTestResult Pact::run_test(std::function<bool(const MockServerHandle*)> callback) const {
    MockServerHandle mockServer(this->pact);
    PactTestResult result;

    if (mockServer.started_ok()) {
      try {
        bool callback_result = callback(&mockServer);
        bool mock_server_result = pactffi_mock_server_matched(mockServer.get_port());
        if (callback_result && mock_server_result) {
          auto write_result = pactffi_write_pact_file(mockServer.get_port(), this->pact_directory.data(), false);
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
        result.add_state(TestResultState::UserCodeFailed, e.what(), boost::current_exception_diagnostic_information());
      } catch (...) {
        result.add_state(TestResultState::UserCodeFailed);
      }

      if (!pactffi_mock_server_matched(mockServer.get_port())) {
        std::string mismatches = pactffi_mock_server_mismatches(mockServer.get_port());
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
    this->interaction = pactffi_new_interaction(parent->pact, description);
    if (this->interaction == 0) {
      throw std::string("Could not create a new interaction with description ") + description;
    }
  }

  Interaction Interaction::uponReceiving(const char* description) const {
    pactffi_upon_receiving(this->interaction, description);
    return *this;
  }

  Interaction Interaction::given(const char* provider_state) const {
    pactffi_given(this->interaction, provider_state);
    return *this;
  }

  Interaction Interaction::given(const char* provider_state, const std::unordered_map<std::string, std::string>& parameters) const {
    for (auto& p : parameters) {
      pactffi_given_with_param(this->interaction, provider_state, p.first.data(), p.second.data());
    }
    return *this;
  }

  Interaction Interaction::withRequest(const char* method, const char* path) const {
    pactffi_with_request(this->interaction, method, path);
    return *this;
  }

  Interaction Interaction::withQuery(const std::unordered_map<std::string, std::vector<std::string>>& query) const {
    for (auto& q : query) {
      for (auto it = q.second.begin(); it != q.second.end(); it++) {
        pactffi_with_query_parameter(this->interaction, q.first.data(), it - q.second.begin(), it->data());
      }
    }
    return *this;
  }
  
  Interaction Interaction::withHeaders(const std::unordered_map<std::string, std::vector<std::string>>& headers) const {
    for (auto& h : headers) {
      for (auto it = h.second.begin(); it != h.second.end(); it++) {
        pactffi_with_header(this->interaction, InteractionPart_Request, h.first.data(), it - h.second.begin(), it->data());
      }
    }
    return *this;
  }

  Interaction Interaction::withBody(const std::string& body, const std::string& content_type) const {
    pactffi_with_body(this->interaction, InteractionPart_Request, content_type.data(), body.data());
    return *this;
  }

  Interaction Interaction::withJsonBody(pact_consumer::matchers::IMatcher::Ptr body) const {
    pactffi_with_body(this->interaction, InteractionPart_Request, "application/json;charset=UTF-8", 
      body->getJson().data());
    return *this;
  }

  Interaction Interaction::withBinaryFile(const std::string& content_type, const std::filesystem::path& example_file) const {
    std::ifstream file (example_file, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
      pactffi_with_binary_file(this->interaction, InteractionPart_Request, content_type.data(), 
        (const uint8_t*)buffer.data(), size);
      return *this;
    } else {
      throw std::string("Could not read file contents: ") + example_file.string();
    }
  }

  Interaction Interaction::withMultipartFileUpload(const std::string& part_name, const std::string& content_type, const std::filesystem::path& example_file) const {
    auto result = pactffi_with_multipart_file(this->interaction, InteractionPart_Request, content_type.data(), 
      example_file.string().data(), part_name.data());
    if (result.tag == StringResult_Failed) {
      std::string error = result.failed;
      pactffi_free_string(result.failed);
      BOOST_THROW_EXCEPTION(std::runtime_error(error));
    }
    return *this;
  }

  Interaction Interaction::willRespondWith(size_t status) const {
    pactffi_response_status(this->interaction, status);
    return *this;
  }

  Interaction Interaction::withResponseHeaders(const std::unordered_map<std::string, std::vector<std::string>>& headers) const {
    for (auto h : headers) {
      for (auto it = h.second.begin(); it != h.second.end(); it++) {
        pactffi_with_header(this->interaction, InteractionPart_Response, h.first.data(), it - h.second.begin(), it->data());
      }
    }
    return *this;
  }

  Interaction Interaction::withResponseBody(const std::string& body, const std::string& content_type) const {
    pactffi_with_body(this->interaction, InteractionPart_Response, content_type.data(), 
      body.data());
    return *this;
  }

  Interaction Interaction::withResponseJsonBody(pact_consumer::matchers::IMatcher::Ptr body) const {
    pactffi_with_body(this->interaction, InteractionPart_Response, "application/json;charset=UTF-8", 
      body->getJson().data());
    return *this;
  }

  Interaction Interaction::withResponseBinaryFile(const std::string& content_type, const std::filesystem::path& example_file) const {
    std::ifstream file (example_file, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
      pactffi_with_binary_file(this->interaction, InteractionPart_Response, content_type.data(), 
        (const uint8_t*)buffer.data(), size);
      return *this;
    } else {
      throw std::string("Could not read file contents: ") + example_file.string();
    }
  }

  Interaction Interaction::withResponseMultipartFileUpload(const std::string& part_name, const std::string& content_type, const std::filesystem::path& example_file) const {
    auto result = pactffi_with_multipart_file(this->interaction, InteractionPart_Response, content_type.data(), 
      example_file.string().data(), part_name.data());
    if (result.tag == StringResult_Failed) {
      std::string error = result.failed;
      pactffi_free_string(result.failed);
      BOOST_THROW_EXCEPTION(std::runtime_error(error));
    }
    return *this;
  }

  ////////////////////////////////////
  // Mock Server Class
  ////////////////////////////////////

  MockServerHandle::MockServerHandle(PactHandle pact) {
    this->port = pactffi_create_mock_server_for_pact(pact, "127.0.0.1:0", false);
  }

  MockServerHandle::~MockServerHandle() {
    pactffi_cleanup_mock_server(this->port);
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

  void PactTestResult::add_state(TestResultState state, std::string message, std::string ex) {
    this->status |= state;
    this->messages[state] = message;
    this->ex = ex;
  }

  bool PactTestResult::is_ok() const {
    return this->status == 0;
  }

  void PactTestResult::display_errors() {
    if (this->status != 0) {
      std::cout << "\nThe test failed for the following reasons:\n\n";
      if (this->status & TestResultState::Mismatches) {
        if (this->messages[TestResultState::Mismatches].empty()) {
          std::cout << "  * Not all the requests matched\n\n";
        } else {
          std::cout << "  * The following mismatches occurred:\n\n";
          auto mismatches_str = this->messages[TestResultState::Mismatches];
          auto j = json::parse(mismatches_str);
          int i = 1;
          for (json::iterator it = j.begin(); it != j.end(); ++it, i++) {
            auto mismatch = *it;
            std::string type = mismatch["type"];
            if (type == "request-not-found") {
              std::cout << "    " << i << ") An unexpected request was received: " << mismatch["method"] << " " << mismatch["path"] << "\n\n";
            } else if (type == "missing-request") {
              std::cout << "    " << i << ") An expected request was not received: " << mismatch["method"] << " " << mismatch["path"] << "\n\n";
            } else if (type == "request-mismatch") {
              std::cout << "    " << i << ") Mismatched request was received: " << mismatch["method"] << " " << mismatch["path"] << "\n\n";
              auto mismatches = mismatch["mismatches"];
              int m = 1;
              for (json::iterator it = mismatches.begin(); it != mismatches.end(); ++it, m++) {
                auto mismatch_details = *it;
                std::string mismatch_type = mismatch_details["type"];
                if (mismatch_type == "MethodMismatch") {
                  std::string expected = mismatch_details["expected"];
                  std::string actual = mismatch_details["actual"];
                  std::cout << "      " << i << "." << m << ") Method: Expected " << expected << " but was " << actual << "\n";
                } else if (mismatch_type == "PathMismatch") {
                  std::string mismatch = mismatch_details["mismatch"];
                  std::cout << "      " << i << "." << m << ") Path: " << mismatch << "\n";
                } else if (mismatch_type == "StatusMismatch") {
                  std::string expected = mismatch_details["expected"];
                  std::string actual = mismatch_details["actual"];
                  std::cout << "      " << i << "." << m << ") Status: Expected " << expected << " but was " << actual << "\n";
                } else if (mismatch_type == "QueryMismatch") {
                  std::string mismatch = mismatch_details["mismatch"];
                  std::cout << "      " << i << "." << m << ") Query Parameter: " << mismatch << "\n";
                } else if (mismatch_type == "HeaderMismatch") {
                  std::string mismatch = mismatch_details["mismatch"];
                  std::cout << "      " << i << "." << m << ") Header: " << mismatch << "\n";
                } else if (mismatch_type == "BodyTypeMismatch") {
                  std::string expected = mismatch_details["expected"];
                  std::string actual = mismatch_details["actual"];
                  std::cout << "      " << i << "." << m << ") Body Type: Expected " << expected << " but was " << actual << "\n";
                } else if (mismatch_type == "BodyMismatch") {
                  std::string path = mismatch_details["path"];
                  std::string mismatch = mismatch_details["mismatch"];
                  std::cout << "      " << i << "." << m << ") Body: " << path << " - " << mismatch << "\n";
                } else {
                  std::cout << "      " << i << "." << m << ") An unknown type of mismatch occurred: " << mismatch_type << "\n";
                }
              }
            }
          }
        }
      }
      if (this->status & TestResultState::UserCodeFailed) {
        auto message = this->messages[TestResultState::UserCodeFailed];
        if (message.empty()) {
          std::cout << "  * Test callback failed with an exception\n\n";
        } else {
          std::cout << "  * Test callback failed with an exception: " << message << "\n\n";
        }
        if (ex.has_value()) {
          std::cout << "    " << ex.value() << "\n";
        }
      }
      if (this->status & TestResultState::PactFileError) {
        auto message = this->messages[TestResultState::PactFileError];
        if (message.empty()) {
          std::cout << "  * Failed to write the Pact file\n";
        } else {
          std::cout << "  * Failed to write the Pact file: " << message << "\n";
        }
      }
      if (this->status & TestResultState::MockServerFailed) {
        auto message = this->messages[TestResultState::MockServerFailed];
        if (message.empty()) {
          std::cout << "  * Mock server failed to start\n";
        } else {
          std::cout << "  * Mock server failed to start: " << message << "\n";
        }
      }
    }
    std::cout << "\n";
  }
}
