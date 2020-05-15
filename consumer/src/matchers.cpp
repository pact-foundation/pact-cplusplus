#include "matchers.h"
#include <pact_mock_server_ffi.h>
#include <iostream>

namespace pact_consumer::matchers {

  IMatcher::Ptr Object(const std::unordered_map<std::string, IMatcher::Ptr> fields) {
    return std::make_shared<ObjectMatcher>(fields);
  }

  IMatcher::Ptr Integer(long value) {
    return std::make_shared<IntegerMatcher>(value);
  }

  IMatcher::Ptr Integer() {
    return std::make_shared<IntegerMatcher>();
  }

  IMatcher::Ptr Like(long value) {
    return std::make_shared<TypeMatcher<long>>(value);
  }

  IMatcher::Ptr Like(int value) {
    return std::make_shared<TypeMatcher<int>>(value);
  }

  IMatcher::Ptr Like(double value) {
    return std::make_shared<TypeMatcher<double>>(value);
  }

  IMatcher::Ptr Like(std::string value) {
    return std::make_shared<TypeMatcher<std::string>>(value);
  }

  IMatcher::Ptr Like(const char *value) {
    return std::make_shared<TypeMatcher<std::string>>(value);
  }
  
  IMatcher::Ptr Like(bool value) {
    return std::make_shared<TypeMatcher<bool>>(value);
  }

  IMatcher::Ptr DateTime(std::string format, std::string example) {
    return std::make_shared<DateTimeMatcher>(format, example);
  }

  IMatcher::Ptr DateTime(std::string format) {
    return std::make_shared<DateTimeMatcher>(format);
  }

  IMatcher::Ptr Matching(std::string regex, std::string example) {
    return std::make_shared<RegexMatcher>(regex, example);
  }

  IMatcher::Ptr Matching(std::string regex) {
    return std::make_shared<RegexMatcher>(regex);
  }

  IMatcher::Ptr EachLike(const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(1, obj);
  }

  IMatcher::Ptr EachLike(int examples, const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(examples, obj);
  }

  IMatcher::Ptr AtLeastOneLike(const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(1, 1, obj);
  }

  IMatcher::Ptr AtLeastOneLike(int examples, const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(examples, 1, obj);
  }

  json ObjectMatcher::getJson() const {
    auto obj = json::object();

    for (auto field : fields) {
      obj[field.first] = field.second->getJson();
    }

    return obj;
  }

  json IntegerMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "integer";
    if (value.has_value()) {
      j["value"] = value.value();
    } else {
      j["value"] = 101;
      j["pact:generator:type"] = "RandomInt";
    }
    return j;
  }

  template<typename T>
  json TypeMatcher<T>::getJson() const {
    json j;
    j["pact:matcher:type"] = "type";
    j["value"] = value;
    return j;
  }

  json DateTimeMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "timestamp";
    j["format"] = format;
    if (!example.empty()) {
      j["value"] = example;
    } else {
      j["pact:generator:type"] = "DateTime";
      auto result = pact_mock_server_ffi::generate_datetime_string(format.data());
      if (result.tag == pact_mock_server_ffi::StringResult::Tag::Ok) {
        j["value"] = result.ok._0;
      } else {
        std::string error = result.failed._0;
        pact_mock_server_ffi::free_string(result.failed._0);
        BOOST_THROW_EXCEPTION(std::runtime_error(error));
      }
    }
    return j;
  }

  json RegexMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "regex";
    j["regex"] = regex;
    if (!example.empty()) {
      if (pact_mock_server_ffi::check_regex(regex.data(), example.data())) {
        j["value"] = example;
      } else {
        std::ostringstream stringStream;
        stringStream << "regex: Example value '" << example << "' does not match the regular expression '" << regex << "'";
        BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
      }
    } else {
      j["pact:generator:type"] = "Regex";
      auto result = pact_mock_server_ffi::generate_regex_value(regex.data());
      if (result.tag == pact_mock_server_ffi::StringResult::Tag::Ok) {
        std::string value = result.ok._0;
        j["value"] = value;
        pact_mock_server_ffi::free_string(result.ok._0);
      } else {
        BOOST_THROW_EXCEPTION(std::runtime_error(result.failed._0));
      }
    }
    return j;
  }

  json EachlikeMatcher::getJson() const {
    json array = json::array();
    for (unsigned int i = 0; i < examples; i++) {
      array.push_back(obj->getJson());
    }
    json j;
    j["pact:matcher:type"] = "type";
    j["value"] = array;
    if (min > 0) {
      j["min"] = min;
    }
    return j;
  }  
}
