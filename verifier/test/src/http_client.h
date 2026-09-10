#pragma once

#include <curl/curl.h>
#include <mutex>
#include <string>

namespace test_support {

  struct HttpResult {
    long status = 0;
    std::string body;
  };

  inline size_t write_callback(char* data, size_t size, size_t nmemb, void* userdata) {
    static_cast<std::string*>(userdata)->append(data, size * nmemb);
    return size * nmemb;
  }

  inline HttpResult request(const std::string& method, const std::string& url, const std::string& body = "") {
    HttpResult result;
    CURL* curl = curl_easy_init();
    if (curl == nullptr) {
      return result;
    }

    curl_slist* headers = curl_slist_append(nullptr, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result.body);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    if (!body.empty()) {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(body.size()));
    }

    if (curl_easy_perform(curl) == CURLE_OK) {
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &result.status);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return result;
  }
}
