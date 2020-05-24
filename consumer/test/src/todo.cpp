#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include "todo.h"
#include "multipart_parser.h"

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
  http_client client(utility::conversions::to_string_t(serverUrl));

  uri_builder builder(U("/projects"));
  builder.append_query(U("from"), U("today"));
  http_request request;
  request.set_request_uri(builder.to_uri());
  if (format == "xml") {
    request.headers().add(header_names::accept, utility::conversions::to_string_t("application/xml"));
  } else {
    request.headers().add(header_names::accept, utility::conversions::to_string_t("application/json, application/hal+json"));
  }

  auto response = client.request(request);
  if (format == "xml") {
    auto body = response.then([=](http_response response) {
      printf("Received response status code:%u\n", response.status_code());
      return response.extract_string();
    }).then([=](string_t body) {
      return projects;
    }).wait();
  } else {
    auto body = response.then([](http_response response) {
      return response.extract_json();
    }).then([&](json::value body) {
      auto p = body[U("projects")].as_array();
      for (auto it = p.begin(); it != p.end(); ++it) {
        Project p;
        auto j = *it;
        p.id = j.at(U("id")).as_integer();
        p.name = utility::conversions::to_utf8string(j.at(U("name")).as_string());
        p.due = utility::conversions::to_utf8string(j.at(U("due")).as_string());

        auto tasks = j.at(U("tasks")).as_array();
        for (auto it = tasks.begin(); it != tasks.end(); ++it) {
          Task t;
          auto j = *it;
          t.id = j.at(U("id")).as_integer();
          t.name = utility::conversions::to_utf8string(j.at(U("name")).as_string());
          t.done = j.at(U("done")).as_bool();
          p.tasks.push_back(t);
        }

        projects.push_back(p);
      }
      return projects;
    }).wait();
  }

  return projects;
}

bool TodoClient::postImage(unsigned int id, std::string file_path) {
  http_client client(utility::conversions::to_string_t(serverUrl));

  MultipartParser parser;
  parser.AddParameter("Filename", file_path);
  parser.AddFile("file", file_path);
  std::string boundary = parser.boundary();
  std::string body = parser.GenBodyContent();

  std::ostringstream out;
  out << "/projects/" << id << "/images";
  uri_builder builder(utility::conversions::to_string_t(out.str()));
  http_request request(U("POST"));
  request.set_request_uri(builder.to_uri());
  request.set_body(body, "multipart/form-data; boundary=" + boundary);

  return client.request(request).then([=](http_response response) {
      printf("Received response status code:%u\n", response.status_code());
      return status_code() == 201;
    }).wait();
}
