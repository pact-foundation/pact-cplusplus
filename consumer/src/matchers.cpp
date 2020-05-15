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

  IMatcher::Ptr Integer(int value) {
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

  IMatcher::Ptr Date(std::string format, std::string example) {
    return std::make_shared<DateMatcher>(format, example);
  }

  IMatcher::Ptr Date(std::string format) {
    return std::make_shared<DateMatcher>(format);
  }

  IMatcher::Ptr Time(std::string format, std::string example) {
    return std::make_shared<TimeMatcher>(format, example);
  }

  IMatcher::Ptr Time(std::string format) {
    return std::make_shared<TimeMatcher>(format);
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

  IMatcher::Ptr HexValue(const std::string example) {
    return std::make_shared<HexadecimalMatcher>(example);
  }

  IMatcher::Ptr HexValue() {
    return std::make_shared<HexadecimalMatcher>();
  }

  IMatcher::Ptr IPAddress(const std::string example) {
    return std::make_shared<IPAddressMatcher>(example);
  }

  IMatcher::Ptr IPAddress() {
    return std::make_shared<IPAddressMatcher>();
  }

  IMatcher::Ptr Numeric(int example) {
    return std::make_shared<NumberMatcher<int>>(example);
  }

  IMatcher::Ptr Numeric(long example) {
    return std::make_shared<NumberMatcher<long>>(example);
  }

  IMatcher::Ptr Numeric(float example) {
    return std::make_shared<NumberMatcher<float>>(example);
  }

  IMatcher::Ptr Numeric(double example) {
    return std::make_shared<NumberMatcher<double>>(example);
  }

  IMatcher::Ptr Numeric() {
    return std::make_shared<NumberMatcher<long>>();
  }

  IMatcher::Ptr Decimal(float example) {
    return std::make_shared<DecimalMatcher>(example);
  }

  IMatcher::Ptr Decimal(double example) {
    return std::make_shared<DecimalMatcher>(example);
  }

  IMatcher::Ptr Decimal() {
    return std::make_shared<DecimalMatcher>();
  }

  IMatcher::Ptr Uuid(const std::string example) {
    return std::make_shared<UuidMatcher>(example);
  }

  IMatcher::Ptr Uuid() {
    return std::make_shared<UuidMatcher>();
  }

  IMatcher::Ptr AtMostLike(int max, const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(1, 0, max, obj);
  }

  IMatcher::Ptr AtMostLike(int max, int examples, const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(examples, 0, max, obj);
  }

  IMatcher::Ptr MinArrayLike(int min, const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(1, min, obj);
  }

  IMatcher::Ptr MinArrayLike(int min, int examples, const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(examples, min, obj);
  }

  IMatcher::Ptr ConstrainedArrayLike(int min, int max, const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(1, min, max, obj);
  }

  IMatcher::Ptr ConstrainedArrayLike(int min, int max, int examples, const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(examples, min, max, obj);
  }

  IMatcher::Ptr EqualTo(int value) {
    return std::make_shared<EqualsMatcher<int>>(value);
  }

  IMatcher::Ptr EqualTo(long value) {
    return std::make_shared<EqualsMatcher<long>>(value);
  }

  IMatcher::Ptr EqualTo(float value) {
    return std::make_shared<EqualsMatcher<float>>(value);
  }

  IMatcher::Ptr EqualTo(double value) {
    return std::make_shared<EqualsMatcher<double>>(value);
  }

  IMatcher::Ptr EqualTo(std::string value) {
    return std::make_shared<EqualsMatcher<std::string>>(value);
  }

  IMatcher::Ptr EqualTo(const char *value) {
    return std::make_shared<EqualsMatcher<std::string>>(value);
  }

  IMatcher::Ptr EqualTo(bool value) {
    return std::make_shared<EqualsMatcher<bool>>(value);
  }

  IMatcher::Ptr IncludesStr(std::string value) {
    return std::make_shared<IncludesMatcher>(value);
  }

  IMatcher::Ptr NullValue() {
    return std::make_shared<NullMatcher>();
  }

  IMatcher::Ptr Url(std::string basePath, std::vector<IMatcher::Ptr> pathFragments) {
    return std::make_shared<UrlMatcher>(basePath, pathFragments);
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

  json DecimalMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "decimal";
    if (value.has_value()) {
      j["value"] = value.value();
    } else {
      j["value"] = 100.0;
      j["pact:generator:type"] = "RandomDecimalGenerator";
      j["digits"] = 6;
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

  template<typename T>
  json NumberMatcher<T>::getJson() const {
    json j;
    j["pact:matcher:type"] = "number";
    if (value.has_value()) {
      j["value"] = value.value();
    } else {
      j["value"] = 100;
      j["pact:generator:type"] = "RandomDecimal";
      j["digits"] = 6;
    }
    return j;
  }

  template<typename T>
  json EqualsMatcher<T>::getJson() const {
    json j;
    j["pact:matcher:type"] = "equality";
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

  json DateMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "date";
    j["format"] = format;
    if (!example.empty()) {
      j["value"] = example;
    } else {
      j["pact:generator:type"] = "Date";
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

  json TimeMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "time";
    j["format"] = format;
    if (!example.empty()) {
      j["value"] = example;
    } else {
      j["pact:generator:type"] = "Time";
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
    if (min > 0 && max > 0 && min > max) {
      std::ostringstream stringStream;
      stringStream << "eachLike: Min array size " << min << " is greater than the max size " << max;
      BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
    } else if (min > 0 && examples < min) {
      std::ostringstream stringStream;
      stringStream << "eachLike: Number of examples " << examples << " is less than the min size " << min;
      BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
    } else if (max > 0 && examples > max) {
      std::ostringstream stringStream;
      stringStream << "eachLike: Number of examples " << examples << " is greater than the max size " << max;
      BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
    }

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
    if (max > 0) {
      j["max"] = max;
    }
    return j;
  }

  json HexadecimalMatcher::getJson() const {
    json j;
    const char *regex = "[0-9a-fA-F]+";
    j["pact:matcher:type"] = "regex";
    j["regex"] = regex;
    if (!example.empty()) {
      if (pact_mock_server_ffi::check_regex(regex, example.data())) {
        j["value"] = example;
      } else {
        std::ostringstream stringStream;
        stringStream << "regex: Example value '" << example << "' does not match the regular expression '" << regex << "'";
        BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
      }
    } else {
      j["pact:generator:type"] = "Regex";
      auto result = pact_mock_server_ffi::generate_regex_value(regex);
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

  json IPAddressMatcher::getJson() const {
    json j;
    const char *regex = "(\\d{1,3}\\.)+\\d{1,3}";
    j["pact:matcher:type"] = "regex";
    j["regex"] = regex;
    if (!example.empty()) {
      if (pact_mock_server_ffi::check_regex(regex, example.data())) {
        j["value"] = example;
      } else {
        std::ostringstream stringStream;
        stringStream << "regex: Example value '" << example << "' does not match the regular expression '" << regex << "'";
        BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
      }
    } else {
      j["pact:generator:type"] = "Regex";
      auto result = pact_mock_server_ffi::generate_regex_value(regex);
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

  json UuidMatcher::getJson() const {
    json j;
    const char *regex = "[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}";
    j["pact:matcher:type"] = "regex";
    j["regex"] = regex;
    if (!example.empty()) {
      if (pact_mock_server_ffi::check_regex(regex, example.data())) {
        j["value"] = example;
      } else {
        std::ostringstream stringStream;
        stringStream << "regex: Example value '" << example << "' does not match the regular expression '" << regex << "'";
        BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
      }
    } else {
      j["pact:generator:type"] = "Uuid";
      j["value"] = "e2490de5-5bd3-43d5-b7c4-526e33f71304";
    }
    return j;
  }

  json IncludesMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "include";
    j["value"] = value;
    return j;
  }

  json NullMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "null";
    return j;
  }

  json UrlMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "regex";
    std::string regex = ".*";
    for (const auto &p : pathFragments) {
      regex += "\\/" + p->as_regex();
    }
    j["regex"] = regex;
    std::string example = basePath;
    for (const auto &p : pathFragments) {
      example += "/" + p->as_example();
    }
    j["value"] = example;
    return j;
  }
}
