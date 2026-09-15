#include <iostream>
#include <fstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>
#include <consumer.h>
#include "todo.h"

using namespace testing;
using namespace pact_consumer;
using namespace pact_consumer::matchers;
using json = nlohmann::json;

namespace {
  // Reads the interaction with the given description back from the pact file the
  // test wrote, so assertions cover what was recorded rather than what was called.
  json readInteraction(const std::string& description) {
    std::ifstream file("pacts/TodoAppCpp-TodoServiceCpp.json");
    if (!file.is_open()) {
      ADD_FAILURE() << "pact file was not written";
      return json();
    }
    json pact = json::parse(file);
    for (const auto& interaction : pact["interactions"]) {
      if (interaction["description"] == description) {
        return interaction;
      }
    }
    ADD_FAILURE() << "no interaction with description '" << description << "' in pact file";
    return json();
  }
}

TEST(PactConsumerTest, GetJsonProjects) {
  auto provider = pact_consumer::Pact("TodoAppCpp", "TodoServiceCpp");
  provider.withSpecification(PactSpecification_V4);
  provider.pact_directory = "pacts";
  
  std::unordered_map<std::string, std::vector<std::string>> query;
  query["from"] = std::vector<std::string>{"today"};

  std::unordered_map<std::string, std::vector<std::string>> headers;
  headers["Accept"] = std::vector<std::string>{"application/json", "application/hal+json"};

  std::unordered_map<std::string, std::vector<std::string>> res_headers;
  res_headers["Content-Type"] = std::vector<std::string>{"application/json"};

  provider
    .given("i have a list of projects")
    .uponReceiving("a request for projects")
    .withRequest("GET", "/projects")
    .withQuery(query)
    .withHeaders(headers)
    .willRespondWith(200)
    .withResponseHeaders(res_headers)
    .withResponseJsonBody(Object({
      { "projects", EachLike(Object({
        { "id", Integer(3) },
        { "name", Like("Project 1") },
        { "due", DateTime("yyyy-MM-dd'T'HH:mm:ss.SSSX") },
        { "tasks", AtLeastOneLike(4, Object({
          { "id", Integer() },
          { "name", Matching("Task \\d+", "Task 1") },
          { "done", Like(true) }
        }))}
      }))}
    }));

  auto result = provider.run_test([] (auto mock_server) {
    TodoClient todo;
    todo.serverUrl = mock_server->get_url();
    std::cout << "URL: " << todo.serverUrl << "\n";

    std::vector<Project> projects = todo.getProjects();

    EXPECT_THAT(todo.getProjects(), SizeIs(1));
    Project p = projects[0];
    EXPECT_EQ(p.name, "Project 1");
    EXPECT_GE(p.id, 0);
    EXPECT_THAT(p.tasks, SizeIs(4));
    EXPECT_GE(p.tasks[0].id, 0);
    EXPECT_EQ(p.tasks[0].name, "Task 1");

    return ::testing::UnitTest::GetInstance()->current_test_suite()->Passed();
  });
  EXPECT_TRUE(result.is_ok()) << "Test failed";
}

TEST(PactConsumerTest, PutProjectImage) {
  auto provider = pact_consumer::Pact("TodoAppCpp", "TodoServiceCpp");
  provider.withSpecification(PactSpecification_V4);
  provider.pact_directory = "pacts";
  
  provider
    .given("i have a project", std::unordered_map<std::string, std::string> {
        {"id","1001"},
        {"name","Home Chores"}
    })
    .uponReceiving("a request to store an image against the project")
    .withRequest("POST", "/projects/1001/images")
    .withMultipartFileUpload("file", "image/jpeg", "example.jpg")
    .willRespondWith(201);

  auto result = provider.run_test([] (auto mock_server) {
    TodoClient todo;
    todo.serverUrl = mock_server->get_url();
    
    auto result = todo.postImage(1001, "example.jpg");
    EXPECT_EQ(result, true);
    return ::testing::UnitTest::GetInstance()->current_test_suite()->Passed();
  });
  EXPECT_TRUE(result.is_ok()) << "Test failed";
}

TEST(PactConsumerTest, AsyncMessage) {
  auto provider = pact_consumer::Pact("TodoAppCpp", "TodoServiceCpp");
  provider.pact_directory = "pacts";
  provider.withSpecification(PactSpecification_V4);

  provider
    .newMessage("a project created event")
    .given("i have a list of projects")
    .withMetadata("contentType", "application/json")
    .withJsonBody(Object({
      { "id", Integer(1001) },
      { "name", Like("Home Chores") }
    }));

  auto result = provider.run_message_test([] { return true; });
  ASSERT_TRUE(result.is_ok()) << "Test failed";

  auto interaction = readInteraction("a project created event");
  EXPECT_EQ(interaction["type"], "Asynchronous/Messages");
  EXPECT_EQ(interaction["providerStates"][0]["name"], "i have a list of projects");
  EXPECT_EQ(interaction["metadata"]["contentType"], "application/json");
  EXPECT_EQ(interaction["contents"]["content"]["id"], 1001);
  EXPECT_EQ(interaction["contents"]["content"]["name"], "Home Chores");
  EXPECT_EQ(interaction["matchingRules"]["body"]["$.id"]["matchers"][0]["match"], "integer");
  EXPECT_EQ(interaction["matchingRules"]["body"]["$.name"]["matchers"][0]["match"], "type");
}

TEST(PactConsumerTest, SyncMessage) {
  auto provider = pact_consumer::Pact("TodoAppCpp", "TodoServiceCpp");
  provider.pact_directory = "pacts";
  provider.withSpecification(PactSpecification_V4);

  provider
    .newSyncMessage("a request for a project by id")
    .given("i have a list of projects")
    .withMetadata("topic", "projects")
    .withBody("{\"id\": 1001}", "application/json")
    .withResponseBody("{\"id\": 1001, \"name\": \"Home Chores\"}", "application/json")
    .withResponseMetadata("topic", "projects.replies");

  auto result = provider.run_message_test([] { return true; });
  ASSERT_TRUE(result.is_ok()) << "Test failed";

  auto interaction = readInteraction("a request for a project by id");
  EXPECT_EQ(interaction["type"], "Synchronous/Messages");
  EXPECT_EQ(interaction["request"]["contents"]["content"]["id"], 1001);
  EXPECT_EQ(interaction["request"]["metadata"]["contentType"], "application/json");
  EXPECT_EQ(interaction["request"]["metadata"]["topic"], "projects");
  ASSERT_EQ(interaction["response"].size(), 1);
  EXPECT_EQ(interaction["response"][0]["contents"]["content"]["name"], "Home Chores");
  EXPECT_EQ(interaction["response"][0]["metadata"]["contentType"], "application/json");
  EXPECT_EQ(interaction["response"][0]["metadata"]["topic"], "projects.replies");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  init();
  return RUN_ALL_TESTS();
}
