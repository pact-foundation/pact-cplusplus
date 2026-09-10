#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include "todo.h"
#include "multipart_parser.h"

using namespace std;
using json = nlohmann::json;
using web::http::MultipartParser;

namespace {

  struct Response {
    long status_code = 0;
    std::string body;
  };

  size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    auto *out = static_cast<std::string *>(userdata);
    out->append(ptr, size * nmemb);
    return size * nmemb;
  }

  void global_init() {
    static std::once_flag flag;
    std::call_once(flag, [] { curl_global_init(CURL_GLOBAL_DEFAULT); });
  }

  // Owns the easy handle, header list and body buffer for a single request.
  class Request {
    public:
      Request() {
        global_init();
        handle_ = curl_easy_init();
        if (!handle_) {
          throw std::runtime_error("Failed to initialise curl");
        }
      }

      ~Request() {
        if (headers_) {
          curl_slist_free_all(headers_);
        }
        if (handle_) {
          curl_easy_cleanup(handle_);
        }
      }

      Request(const Request &) = delete;
      Request &operator=(const Request &) = delete;

      void add_header(const std::string &header) {
        headers_ = curl_slist_append(headers_, header.c_str());
      }

      void set_post_body(const std::string &body) {
        body_ = body;
        curl_easy_setopt(handle_, CURLOPT_POST, 1L);
        curl_easy_setopt(handle_, CURLOPT_POSTFIELDS, body_.c_str());
        curl_easy_setopt(handle_, CURLOPT_POSTFIELDSIZE, static_cast<long>(body_.size()));
      }

      Response perform(const std::string &url) {
        Response response;
        curl_easy_setopt(handle_, CURLOPT_URL, url.c_str());
        curl_easy_setopt(handle_, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(handle_, CURLOPT_WRITEDATA, &response.body);
        curl_easy_setopt(handle_, CURLOPT_FOLLOWLOCATION, 1L);
        // Avoid hanging indefinitely if the mock server never responds (seen on Windows CI).
        curl_easy_setopt(handle_, CURLOPT_CONNECTTIMEOUT, 10L);
        curl_easy_setopt(handle_, CURLOPT_TIMEOUT, 30L);
        if (headers_) {
          curl_easy_setopt(handle_, CURLOPT_HTTPHEADER, headers_);
        }

        CURLcode result = curl_easy_perform(handle_);
        if (result != CURLE_OK) {
          throw std::runtime_error(std::string("HTTP request failed: ") + curl_easy_strerror(result));
        }

        curl_easy_getinfo(handle_, CURLINFO_RESPONSE_CODE, &response.status_code);
        return response;
      }

    private:
      CURL *handle_ = nullptr;
      curl_slist *headers_ = nullptr;
      std::string body_;
  };
}

TodoClient::TodoClient() {
  serverUrl = "http://localhost:8080";
}

vector<Project> TodoClient::getProjects(string format) {
  vector<Project> projects;

  Request request;
  if (format == "xml") {
    request.add_header("Accept: application/xml");
  } else {
    request.add_header("Accept: application/json, application/hal+json");
  }

  Response response = request.perform(serverUrl + "/projects?from=today");
  std::printf("Received response status code:%ld\n", response.status_code);

  if (format == "xml") {
    return projects;
  }

  json body = json::parse(response.body);
  for (const auto &j : body.at("projects")) {
    Project p;
    p.id = j.at("id").get<unsigned int>();
    p.name = j.at("name").get<std::string>();
    p.due = j.at("due").get<std::string>();

    for (const auto &task : j.at("tasks")) {
      Task t;
      t.id = task.at("id").get<unsigned int>();
      t.name = task.at("name").get<std::string>();
      t.done = task.at("done").get<bool>();
      p.tasks.push_back(t);
    }

    projects.push_back(p);
  }

  return projects;
}

bool TodoClient::postImage(unsigned int id, std::string file_path) {
  MultipartParser parser;
  parser.AddParameter("Filename", file_path);
  parser.AddFile("file", file_path);
  std::string boundary = parser.boundary();
  std::string body = parser.GenBodyContent();

  std::ostringstream url;
  url << serverUrl << "/projects/" << id << "/images";

  Request request;
  request.add_header("Content-Type: multipart/form-data; boundary=" + boundary);
  request.set_post_body(body);

  Response response = request.perform(url.str());
  printf("Received response status code:%ld\n", response.status_code);

  return response.status_code == 201;
}
