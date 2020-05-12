#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <consumer.h>
#include "todo.h"

using namespace testing;
using namespace pact_consumer;

TEST(PactConsumerTest, GetJsonProjects) {
  auto provider = Pact("TodoAppCpp", "TodoServiceCpp");
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
    .withResponseJsonBody([](auto body) {
      body->eachLike("projects", [](auto project) {
        (*project)
          .integer("id")
          .string("name", "Project 1")
          .datetime("due", "yyyy-MM-dd'T'HH:mm:ss.SSSX", "2016-02-11T09:46:56.023Z")
          .atLeastOneLike("tasks", 4, [](auto task) {
            (*task)
              .integer("id")
              .string("name", "Task 1")
              .boolean("done", true);
          });
      });
    });

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
  auto provider = Pact("TodoAppCpp", "TodoServiceCpp");
  provider.pact_directory = "pacts";
  
  provider
    .given("i have a project", std::unordered_map<std::string, std::string> {
        {"id","1001"},
        {"name","Home Chores"}
    })
    .uponReceiving("a request to store an image against the project")
    .withRequest("POST", "/projects/1001/images")
    .withBinaryFile("image/jpeg", "example.jpg")
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  init();
  return RUN_ALL_TESTS();
}
