#include <gtest/gtest.h>

#include <map>
#include <mutex>
#include <string>

#include <nlohmann/json.hpp>

#include "verifier.h"

using json = nlohmann::json;
using namespace pact_verifier;

namespace {

  /**
   * A stub provider that serves users out of an in-memory map. The map is
   * populated by the provider state handlers registered on the verifier, in the
   * same way a real provider would seed a test database.
   */
  class StubProvider {
    public:
      StubProvider() {
        server.set_handler([this](const http::Request& request, http::Response& response) {
          handle(request, response);
        });
      }

      bool start() { return server.start(); }
      void stop() { server.stop(); }
      uint16_t port() const { return server.get_port(); }

      void add_user(int id, const std::string& name) {
        std::lock_guard<std::mutex> guard(lock);
        users[id] = name;
      }

      void remove_user(int id) {
        std::lock_guard<std::mutex> guard(lock);
        users.erase(id);
      }

      void clear() {
        std::lock_guard<std::mutex> guard(lock);
        users.clear();
      }

    private:
      void handle(const http::Request& request, http::Response& response) {
        if (request.method != "GET" || request.path.rfind("/users/", 0) != 0) {
          response.status = 404;
          response.body = R"({"error":"Not found"})";
          return;
        }

        int id = 0;
        try {
          id = std::stoi(request.path.substr(7));
        } catch (const std::exception&) {
          response.status = 400;
          response.body = R"({"error":"Invalid user id"})";
          return;
        }

        std::lock_guard<std::mutex> guard(lock);
        auto user = users.find(id);
        if (user == users.end()) {
          response.status = 404;
          response.body = R"({"error":"User not found"})";
          return;
        }

        response.status = 200;
        response.body = json({{"id", user->first}, {"name", user->second}}).dump();
      }

      http::Server server;
      std::mutex lock;
      std::map<int, std::string> users;
  };

  const char* PACT_FILE = "pacts/cpp-consumer-cpp-provider.json";
}

TEST(VerifierTest, VerifiesAPactFileUsingHostedProviderStates) {
  init();

  StubProvider provider;
  ASSERT_TRUE(provider.start());

  Verifier verifier;
  verifier
    .set_provider_info("cpp-provider", "http", "127.0.0.1", provider.port(), "/")
    .add_file_source(PACT_FILE)
    .set_coloured_output(false)
    .set_strip_ansi_from_output(true)
    .add_state_handler("the user exists", [&provider](const ProviderStateRequest& request) {
      if (request.action == StateAction::Setup) {
        provider.add_user(static_cast<int>(request.params.get_int("id").value_or(0)),
          request.params.get_or("name", ""));
      } else {
        provider.clear();
      }
    })
    .add_state_handler("the user does not exist", [&provider](const ProviderStateRequest& request) {
      provider.remove_user(static_cast<int>(request.params.get_int("id").value_or(0)));
    });

  auto result = verifier.execute();
  if (!result.is_ok()) {
    result.display();
  }

  EXPECT_TRUE(result.is_ok());
  EXPECT_EQ(0, result.result_code());
  EXPECT_FALSE(result.json().empty());

  provider.stop();
}

TEST(VerifierTest, FailsWhenTheProviderDoesNotHonourTheContract) {
  StubProvider provider;
  ASSERT_TRUE(provider.start());

  Verifier verifier;
  verifier
    .set_provider_info("cpp-provider", "http", "127.0.0.1", provider.port(), "/")
    .add_file_source(PACT_FILE)
    .set_coloured_output(false)
    .set_strip_ansi_from_output(true)
    .set_filter_info("a request for user 1")
    // The state handler deliberately seeds the wrong name
    .add_state_handler("the user exists", [&provider](const ProviderStateRequest& request) {
      provider.add_user(static_cast<int>(request.params.get_int("id").value_or(0)), "Bob");
    });

  auto result = verifier.execute();

  EXPECT_FALSE(result.is_ok());
  EXPECT_NE(result.output().find("Alice"), std::string::npos);

  provider.stop();
}

TEST(VerifierTest, TreatsNoPactsAsSuccessWhenConfiguredTo) {
  Verifier verifier;
  verifier
    .set_provider_info("provider-with-no-pacts")
    .add_directory_source("pacts")
    .set_coloured_output(false)
    .set_no_pacts_is_error(false);

  EXPECT_TRUE(verifier.execute().is_ok());
}

TEST(VerifierTest, RejectsInvalidConsumerVersionSelectors) {
  Verifier verifier;

  BrokerSelectorOptions options;
  options.url = "http://localhost:9292";
  options.consumer_version_selectors = {"this is not json"};

  EXPECT_FALSE(verifier.add_broker_source_with_selectors(options));
}
