#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cstring>
#include <string>
#include "todo.h"

using namespace std;
// using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
// using namespace concurrency::streams;       // Asynchronous streams

TodoClient::TodoClient() {
  serverUrl = "http://localhost:8080";
}

/*
getProjects: async (format = "json") => {
    return axios
      .get(serverUrl + "/projects?from=today", {
        headers: {
          Accept: "application/" + format,
        },
      })
      .then(response => {
        console.log("todo response:")
        eyes.inspect(response.data)
        if (format === "xml") {
          const result = JSON.parse(parser.toJson(response.data))
          console.dir(result, { depth: 10 })
          return R.path(["ns1:projects"], result)
        }
        return response.data
      })
      .catch(error => {
        console.log("todo error", error.message)
        return Promise.reject(error)
      })
  },
*/

vector<Project> TodoClient::getProjects(string format) {
  vector<Project> projects;

  // Create http_client to send the request.
  http_client client(U(serverUrl));

  uri_builder builder(U("/projects"));
  builder.append_query(U("from"), U("today"));
  http_request request;
  request.set_request_uri(builder.to_uri());
  request.headers().add(header_names::accept, "application/" + format);

  auto response = client.request(request);
  if (format == "xml") {
    auto body = response.then([=](http_response response) {
      printf("Received response status code:%u\n", response.status_code());
      return response.extract_string();
    }).then([=](string body) {
      printf("Body: %s\n", body.data());
      return projects;
    }).wait();
  } else {
    auto body = response.then([=](http_response response) {
      printf("Received response status code:%u\n", response.status_code());
      return response.extract_json();
    }).then([=](json::value body) {
      printf("Body: %s\n", body.serialize().data());
      return projects;
    }).wait();
  }

  return projects;
}
