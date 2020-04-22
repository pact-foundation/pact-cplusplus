#include <iostream>
#include <gtest/gtest.h>
#include <consumer.h>
#include "todo.h"

using namespace std;
using namespace pact_consumer;

TEST(PactConsumerTest, GetJsonProjects) {
  auto provider = Pact("TodoAppCpp", "TodoServiceCpp");
  // dir: path.resolve(process.cwd(), "pacts"),
  //   logLevel: "INFO",
  // })

  unordered_map<string, vector<string>> query;
  query["from"] = vector<string>{"today"};

  unordered_map<string, vector<string>> headers;
  headers["Accept"] = vector<string>{"application/json"};

  unordered_map<string, vector<string>> res_headers;
  res_headers["Content-Type"] = vector<string>{"application/json"};

  provider
    .given("i have a list of projects")
    .uponReceiving("a request for projects")
    .withRequest("GET", "/projects")
    .withQuery(query)
    .withHeaders(headers)
    .willRespondWith(200)
    .withResponseHeaders(res_headers)
    .withResponseJsonBody([](auto body) {
      body.eachLike("projects", [](auto project) {
        // project
        //   .integer("id", 1)
        //   .string("name", "Project 1")
        //   .timestamp("due", "yyyy-MM-dd'T'HH:mm:ss.SSSX", "2016-02-11T09:46:56.023Z");

      });
    });
  //     body: eachLike({
  //       id: integer(1),
  //       name: string("Project 1"),
  //       due: timestamp(
  //         "yyyy-MM-dd'T'HH:mm:ss.SSSX",
  //         "2016-02-11T09:46:56.023Z"
  //       ),
  //       tasks: atLeastOneLike(
  //         {
  //           id: integer(),
  //           name: string("Do the laundry"),
  //           done: boolean(true),
  //         },
  //         4
  //       ),
  //     }),
  //   });

  // provider.run_test([=] (pact_consumer::MockServer mock_server) {
  //   TodoClient todo(mock_server.get_url());

  //   vector<Project> projects = todo.getProjects();

  //   EXPECT_EQ(todo.serverUrl, "1");
  // });
}

// TEST(PactConsumerTest, GetXmlProjects) {
//   TodoClient todo;

//   cout << "URL: " << todo.serverUrl << "\n";

//   vector<Project> projects = todo.getProjects("xml");

//   EXPECT_EQ(todo.serverUrl, "1");
// }
