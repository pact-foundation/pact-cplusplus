#include <iostream>
#include <gtest/gtest.h>
#include <consumer.h>
#include "todo.h"

using namespace std;

TEST(PactConsumerTest, GetJsonProjects) {
  auto provider = pact_consumer::Pact("TodoAppCpp", "TodoServiceCpp");
  // dir: path.resolve(process.cwd(), "pacts"),
  //   logLevel: "INFO",
  // })

  provider
    .given("i have a list of projects")
    .uponReceiving("a request for projects");
  //   .withRequest({
  //     method: "GET",
  //     path: "/projects",
  //     query: { from: "today" },
  //     headers: { Accept: "application/json" },
  //   })
  //   .willRespondWith({
  //     status: 200,
  //     headers: { "Content-Type": "application/json" },
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
