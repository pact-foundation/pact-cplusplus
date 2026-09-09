#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include "http_client.h"
#include "verifier.h"

using json = nlohmann::json;
using namespace pact_verifier;

TEST(HttpServerTest, ServesRequestsAndExposesTheRequestDetails) {
  http::Server server;
  server.set_handler([](const http::Request& request, http::Response& response) {
    json body = {
      {"method", request.method},
      {"path", request.path},
      {"body", request.body},
      {"answer", request.query_params.count("answer") ? request.query_params.at("answer") : ""}
    };
    response.body = body.dump();
  });

  ASSERT_TRUE(server.start());
  ASSERT_GT(server.get_port(), 0);

  auto result = test_support::request("POST", server.get_url() + "/echo?answer=42", R"({"hello":"world"})");
  ASSERT_EQ(200, result.status);

  json body = json::parse(result.body);
  EXPECT_EQ("POST", body["method"]);
  EXPECT_EQ("/echo", body["path"]);
  EXPECT_EQ(R"({"hello":"world"})", body["body"]);
  EXPECT_EQ("42", body["answer"]);

  server.stop();
  EXPECT_FALSE(server.is_running());
}

TEST(ProviderStateServerTest, InvokesTheHandlerForSetupAndTeardown) {
  ProviderStateServer states;

  std::vector<std::string> calls;
  states.add_state_handler("the user exists", [&calls](const ProviderStateRequest& request) {
    calls.push_back((request.action == StateAction::Setup ? "setup:" : "teardown:") + request.state +
      ":" + std::to_string(request.params.get_int("id").value_or(-1)) +
      ":" + request.params.get_or("name", "<none>"));
  });

  ASSERT_TRUE(states.start());

  auto setup = test_support::request("POST", states.get_url(),
    R"({"state":"the user exists","action":"setup","params":{"id":7,"name":"Alice"}})");
  EXPECT_EQ(200, setup.status);

  auto teardown = test_support::request("POST", states.get_url(),
    R"({"state":"the user exists","action":"teardown","params":{"id":7,"name":"Alice"}})");
  EXPECT_EQ(200, teardown.status);

  ASSERT_EQ(2u, calls.size());
  EXPECT_EQ("setup:the user exists:7:Alice", calls[0]);
  EXPECT_EQ("teardown:the user exists:7:Alice", calls[1]);

  states.stop();
}

TEST(ProviderStateServerTest, ReturnsTheValuesFromAValueReturningHandler) {
  ProviderStateServer states;
  states.add_state_handler_with_values("a user is created", [](const ProviderStateRequest&) {
    return json({{"id", 100}}).dump();
  });

  ASSERT_TRUE(states.start());

  auto result = test_support::request("POST", states.get_url(),
    R"({"state":"a user is created","action":"setup"})");
  ASSERT_EQ(200, result.status);
  EXPECT_EQ(100, json::parse(result.body)["id"]);

  states.stop();
}

TEST(ProviderStateServerTest, ReadsStateDataFromQueryParametersWhenThereIsNoBody) {
  ProviderStateServer states;

  std::string seen_state;
  std::string seen_id;
  states.add_state_handler("the user exists", [&](const ProviderStateRequest& request) {
    seen_state = request.state;
    seen_id = request.params.get_or("id", "");
  });

  ASSERT_TRUE(states.start());

  auto result = test_support::request("GET", states.get_url() + "?state=the%20user%20exists&action=setup&id=9");
  EXPECT_EQ(200, result.status);
  EXPECT_EQ("the user exists", seen_state);
  EXPECT_EQ("9", seen_id);

  states.stop();
}

TEST(ProviderStateServerTest, FailsWhenThereIsNoHandlerForTheState) {
  ProviderStateServer states;
  ASSERT_TRUE(states.start());

  auto result = test_support::request("POST", states.get_url(),
    R"({"state":"some unhandled state","action":"setup"})");
  EXPECT_EQ(500, result.status);

  states.set_unknown_state_is_error(false);
  result = test_support::request("POST", states.get_url(),
    R"({"state":"some unhandled state","action":"setup"})");
  EXPECT_EQ(200, result.status);

  states.stop();
}

TEST(ProviderStateParamsTest, ExposesTypedAccessors) {
  ProviderStateParams params(R"({"id":1,"name":"Alice","active":true,"score":1.5,"tags":["a"]})");

  EXPECT_TRUE(params.contains("id"));
  EXPECT_FALSE(params.contains("missing"));
  EXPECT_EQ(1, params.get_int("id").value());
  EXPECT_EQ("Alice", params.get_or("name", ""));
  EXPECT_TRUE(params.get_bool("active").value());
  EXPECT_DOUBLE_EQ(1.5, params.get_double("score").value());
  EXPECT_EQ(R"(["a"])", params.get_or("tags", ""));
  EXPECT_FALSE(params.get_int("name").has_value());
  EXPECT_FALSE(params.get("missing").has_value());
}

TEST(ProviderStateParamsTest, ToleratesInvalidJson) {
  ProviderStateParams params("not json");
  EXPECT_EQ("{}", params.json());
  EXPECT_TRUE(params.values().empty());
}
