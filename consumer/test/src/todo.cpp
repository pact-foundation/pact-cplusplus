#define _TURN_OFF_PLATFORM_STRING
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cstring>
#include <string>
#include "todo.h"

using namespace std;
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features


TodoClient::TodoClient() {
  serverUrl = "http://localhost:8080";
}

vector<Project> TodoClient::getProjects(string format) {
  vector<Project> projects;

  // Create http_client to send the request.
  http_client client(serverUrl);

  uri_builder builder("/projects");
  builder.append_query("from", "today");
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
    auto body = response.then([](http_response response) {
      return response.extract_json();
    }).then([&](json::value body) {
      auto p = body["projects"].as_array();
      for (auto it = p.begin(); it != p.end(); ++it) {
        Project p;
        auto j = *it;
        p.id = j.at("id").as_integer();
        p.name = j.at("name").as_string();
        p.due = j.at("due").as_string();

        auto tasks = j.at("tasks").as_array();
        for (auto it = tasks.begin(); it != tasks.end(); ++it) {
          Task t;
          auto j = *it;
          t.id = j.at("id").as_integer();
          t.name = j.at("name").as_string();
          t.done = j.at("done").as_bool();
          p.tasks.push_back(t);
        }

        projects.push_back(p);
      }
      return projects;
    }).wait();
  }

  return projects;
}
