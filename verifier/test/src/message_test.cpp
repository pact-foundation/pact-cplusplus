#include <gtest/gtest.h>

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "verifier.h"

using json = nlohmann::json;
using namespace pact_verifier;

namespace {
  const char* MESSAGE_PACT_FILE = "pacts/cpp-message-consumer-cpp-message-provider.json";
}

TEST(MessageVerifierTest, VerifiesAsynchronousAndSynchronousMessages) {
  init();

  std::string seeded_user;
  std::vector<std::string> handled;

  Verifier verifier;
  verifier
    .set_provider_info("cpp-message-provider")
    .add_file_source(MESSAGE_PACT_FILE)
    .set_coloured_output(false)
    .set_strip_ansi_from_output(true)
    .add_state_handler("a user exists", [&seeded_user](const ProviderStateRequest& request) {
      seeded_user = request.params.get_or("name", "");
    })
    .add_message_handler("a user created event", [&](const MessageRequest& request) {
      handled.push_back(request.description);
      EXPECT_FALSE(request.synchronous);
      EXPECT_EQ(1u, request.provider_states.size());
      EXPECT_EQ("a user exists", request.provider_states.front().name);

      return ProviderMessage(json({{"id", 1}, {"name", seeded_user}}).dump())
        .with_metadata("topic", "users");
    })
    .add_message_handler("a request for user 1", [&](const MessageRequest& request) {
      handled.push_back(request.description);
      EXPECT_TRUE(request.synchronous);
      EXPECT_EQ("application/json", request.request_content_type);

      int id = json::parse(request.request_body)["id"].get<int>();
      return ProviderMessage(json({{"id", id}, {"name", "Alice"}}).dump());
    });

  auto result = verifier.execute();
  if (!result.is_ok()) {
    result.display();
  }

  EXPECT_TRUE(result.is_ok());
  EXPECT_EQ(2u, handled.size());
}

TEST(MessageVerifierTest, FailsWhenTheMessageDoesNotMatchTheContract) {
  Verifier verifier;
  verifier
    .set_provider_info("cpp-message-provider")
    .add_file_source(MESSAGE_PACT_FILE)
    .set_coloured_output(false)
    .set_strip_ansi_from_output(true)
    .set_filter_info("a user created event")
    .set_default_state_handler([](const ProviderStateRequest&) {})
    .add_message_handler("a user created event", [](const MessageRequest&) {
      return ProviderMessage(R"({"id":1,"name":"Bob"})").with_metadata("topic", "users");
    });

  auto result = verifier.execute();

  EXPECT_FALSE(result.is_ok());
  EXPECT_NE(result.output().find("Alice"), std::string::npos);
}

TEST(MessageVerifierTest, FailsWhenTheMessageMetadataDoesNotMatch) {
  Verifier verifier;
  verifier
    .set_provider_info("cpp-message-provider")
    .add_file_source(MESSAGE_PACT_FILE)
    .set_coloured_output(false)
    .set_strip_ansi_from_output(true)
    .set_filter_info("a user created event")
    .set_default_state_handler([](const ProviderStateRequest&) {})
    .add_message_handler("a user created event", [](const MessageRequest&) {
      return ProviderMessage(R"({"id":1,"name":"Alice"})").with_metadata("topic", "orders");
    });

  auto result = verifier.execute();

  EXPECT_FALSE(result.is_ok());
}

TEST(MessageProviderServerTest, ReturnsTheMessageBodyAndMetadata) {
  MessageProviderServer messages;
  messages.set_default_handler([](const MessageRequest& request) {
    return ProviderMessage(json({{"echo", request.description}}).dump())
      .with_metadata("topic", "users")
      .with_json_metadata("partition", "3");
  });

  ASSERT_TRUE(messages.start());
  EXPECT_EQ("/__pact/message", messages.get_path());

  messages.stop();
}
