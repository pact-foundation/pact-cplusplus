#include "matchers.h"
extern "C" {
#include <pact.h>
}
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace pact_consumer::matchers {

  IMatcher::Ptr Object(const std::unordered_map<std::string, IMatcher::Ptr>& fields) {
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

  IMatcher::Ptr Like(const std::string& value) {
    return std::make_shared<TypeMatcher<std::string>>(value);
  }

  IMatcher::Ptr Like(const char *value) {
    return std::make_shared<TypeMatcher<std::string>>(value);
  }
  
  IMatcher::Ptr Like(bool value) {
    return std::make_shared<TypeMatcher<bool>>(value);
  }

  IMatcher::Ptr DateTime(const std::string& format, const std::string& example) {
    return std::make_shared<DateTimeMatcher>(format, example);
  }

  IMatcher::Ptr DateTime(const std::string& format) {
    return std::make_shared<DateTimeMatcher>(format);
  }

  IMatcher::Ptr Date(const std::string& format, const std::string& example) {
    return std::make_shared<DateMatcher>(format, example);
  }

  IMatcher::Ptr Date(const std::string& format) {
    return std::make_shared<DateMatcher>(format);
  }

  IMatcher::Ptr Time(const std::string& format, const std::string& example) {
    return std::make_shared<TimeMatcher>(format, example);
  }

  IMatcher::Ptr Time(const std::string& format) {
    return std::make_shared<TimeMatcher>(format);
  }

  IMatcher::Ptr Matching(const std::string& regex, const std::string& example) {
    return std::make_shared<RegexMatcher>(regex, example);
  }

  IMatcher::Ptr Matching(const std::string& regex) {
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

  IMatcher::Ptr HexValue(const std::string& example) {
    return std::make_shared<HexadecimalMatcher>(example);
  }

  IMatcher::Ptr HexValue() {
    return std::make_shared<HexadecimalMatcher>();
  }

  IMatcher::Ptr IPAddress(const std::string& example) {
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

  IMatcher::Ptr Uuid(const std::string& example) {
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
    return std::make_shared<EachlikeMatcher>(min, min, obj);
  }

  IMatcher::Ptr MinArrayLike(int min, int examples, const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(examples, min, obj);
  }

  IMatcher::Ptr ConstrainedArrayLike(int min, int max, const IMatcher::Ptr obj) {
    return std::make_shared<EachlikeMatcher>(min, min, max, obj);
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

  IMatcher::Ptr EqualTo(const std::string& value) {
    return std::make_shared<EqualsMatcher<std::string>>(value);
  }

  IMatcher::Ptr EqualTo(const char *value) {
    return std::make_shared<EqualsMatcher<std::string>>(value);
  }

  IMatcher::Ptr EqualTo(bool value) {
    return std::make_shared<EqualsMatcher<bool>>(value);
  }

  IMatcher::Ptr IncludesStr(const std::string& value) {
    return std::make_shared<IncludesMatcher>(value);
  }

  IMatcher::Ptr NullValue() {
    return std::make_shared<NullMatcher>();
  }

  IMatcher::Ptr Url(const std::string& basePath, const std::vector<IMatcher::Ptr>& pathFragments) {
    return std::make_shared<UrlMatcher>(basePath, pathFragments);
  }

  IMatcher::Ptr Url(const std::vector<IMatcher::Ptr>& pathFragments) {
    return std::make_shared<UrlMatcher>("", pathFragments);
  }

  IMatcher::Ptr ArrayContaining(const std::vector<IMatcher::Ptr>& variants) {
    return std::make_shared<ArrayContainsMatcher>(variants);
  }

  std::string ObjectMatcher::getJson() const {
    auto obj = json::object();

    for (auto field : fields) {
      obj[field.first] = json::parse(field.second->getJson());
    }

    return obj.dump();
  }

  std::string IntegerMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "integer";
    if (value.has_value()) {
      j["value"] = value.value();
    } else {
      j["value"] = 101;
      j["pact:generator:type"] = "RandomInt";
    }
    return j.dump();
  }

  std::string DecimalMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "decimal";
    if (value.has_value()) {
      j["value"] = value.value();
    } else {
      j["value"] = 100.0;
      j["pact:generator:type"] = "RandomDecimalGenerator";
      j["digits"] = 6;
    }
    return j.dump();
  }

  template<typename T>
  std::string TypeMatcher<T>::getJson() const {
    json j;
    j["pact:matcher:type"] = "type";
    j["value"] = value;
    return j.dump();
  }

  template<typename T>
  std::string NumberMatcher<T>::getJson() const {
    json j;
    j["pact:matcher:type"] = "number";
    if (value.has_value()) {
      j["value"] = value.value();
    } else {
      j["value"] = 100;
      j["pact:generator:type"] = "RandomDecimal";
      j["digits"] = 6;
    }
    return j.dump();
  }

  template<typename T>
  std::string EqualsMatcher<T>::getJson() const {
    json j;
    j["pact:matcher:type"] = "equality";
    j["value"] = value;
    return j.dump();
  }

  std::string DateTimeMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "timestamp";
    j["format"] = format;
    if (!example.empty()) {
      j["value"] = example;
    } else {
      j["pact:generator:type"] = "DateTime";
      auto result = pactffi_generate_datetime_string(format.data());
      if (result.tag == StringResult_Ok) {
        j["value"] = result.ok;
        pactffi_free_string(result.ok);
      } else {
        std::string error = result.failed;
        pactffi_free_string(result.failed);
        BOOST_THROW_EXCEPTION(std::runtime_error(error));
      }
    }
    return j.dump();
  }

  std::string DateMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "date";
    j["format"] = format;
    if (!example.empty()) {
      j["value"] = example;
    } else {
      j["pact:generator:type"] = "Date";
      auto result = pactffi_generate_datetime_string(format.data());
      if (result.tag == StringResult_Ok) {
        j["value"] = result.ok;
        pactffi_free_string(result.ok);
      } else {
        std::string error = result.failed;
        pactffi_free_string(result.failed);
        BOOST_THROW_EXCEPTION(std::runtime_error(error));
      }
    }
    return j.dump();
  }

  std::string TimeMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "time";
    j["format"] = format;
    if (!example.empty()) {
      j["value"] = example;
    } else {
      j["pact:generator:type"] = "Time";
      auto result = pactffi_generate_datetime_string(format.data());
      if (result.tag == StringResult_Ok) {
        j["value"] = result.ok;
        pactffi_free_string(result.ok);
      } else {
        std::string error = result.failed;
        pactffi_free_string(result.failed);
        BOOST_THROW_EXCEPTION(std::runtime_error(error));
      }
    }
    return j.dump();
  }

  std::string RegexMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "regex";
    j["regex"] = regex;
    if (!example.empty()) {
      if (pactffi_check_regex(regex.data(), example.data())) {
        j["value"] = example;
      } else {
        std::ostringstream stringStream;
        stringStream << "regex: Example value '" << example << "' does not match the regular expression '" << regex << "'";
        BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
      }
    } else {
      j["pact:generator:type"] = "Regex";
      auto result = pactffi_generate_regex_value(regex.data());
      if (result.tag == StringResult_Ok) {
        std::string value = result.ok;
        j["value"] = value;
        pactffi_free_string(result.ok);
      } else {
        std::string error = result.failed;
        pactffi_free_string(result.failed);
        BOOST_THROW_EXCEPTION(std::runtime_error(error));
      }
    }
    return j.dump();
  }

  std::string EachlikeMatcher::getJson() const {
    if (min && min.value() > 0 && max && max.value() > 0 && min.value() > max.value()) {
      std::ostringstream stringStream;
      stringStream << "eachLike: Min array size " << min.value() << " is greater than the max size " << max.value();
      BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
    } else if (min && min.value() > 0 && examples < min.value()) {
      std::ostringstream stringStream;
      stringStream << "eachLike: Number of examples " << examples << " is less than the min size " << min.value();
      BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
    } else if (max && max.value() > 0 && examples > max.value()) {
      std::ostringstream stringStream;
      stringStream << "eachLike: Number of examples " << examples << " is greater than the max size " << max.value();
      BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
    }

    json array = json::array();
    json obj_json = json::parse(obj->getJson());
    for (unsigned int i = 0; i < examples; i++) {
      array.push_back(obj_json);
    }
    json j;
    j["pact:matcher:type"] = "type";
    j["value"] = array;
    if (min && min.value() > 0) {
      j["min"] = min.value();
    }
    if (max && max.value() > 0) {
      j["max"] = max.value();
    }
    return j.dump();
  }

  std::string HexadecimalMatcher::getJson() const {
    json j;
    const char *regex = "[0-9a-fA-F]+";
    j["pact:matcher:type"] = "regex";
    j["regex"] = regex;
    if (!example.empty()) {
      if (pactffi_check_regex(regex, example.data())) {
        j["value"] = example;
      } else {
        std::ostringstream stringStream;
        stringStream << "regex: Example value '" << example << "' does not match the regular expression '" << regex << "'";
        BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
      }
    } else {
      j["pact:generator:type"] = "Regex";
      auto result = pactffi_generate_regex_value(regex);
      if (result.tag == StringResult_Ok) {
        std::string value = result.ok;
        j["value"] = value;
        pactffi_free_string(result.ok);
      } else {
        std::string error = result.failed;
        pactffi_free_string(result.failed);
        BOOST_THROW_EXCEPTION(std::runtime_error(error));
      }
    }
    return j.dump();
  }

  std::string IPAddressMatcher::getJson() const {
    json j;
    const char *regex = "(\\d{1,3}\\.)+\\d{1,3}";
    j["pact:matcher:type"] = "regex";
    j["regex"] = regex;
    if (!example.empty()) {
      if (pactffi_check_regex(regex, example.data())) {
        j["value"] = example;
      } else {
        std::ostringstream stringStream;
        stringStream << "regex: Example value '" << example << "' does not match the regular expression '" << regex << "'";
        BOOST_THROW_EXCEPTION(std::runtime_error(stringStream.str()));
      }
    } else {
      j["pact:generator:type"] = "Regex";
      auto result = pactffi_generate_regex_value(regex);
      if (result.tag == StringResult_Ok) {
        std::string value = result.ok;
        j["value"] = value;
        pactffi_free_string(result.ok);
      } else {
        std::string error = result.failed;
        pactffi_free_string(result.failed);
        BOOST_THROW_EXCEPTION(std::runtime_error(error));
      }
    }
    return j.dump();
  }

  std::string UuidMatcher::getJson() const {
    json j;
    const char *regex = "[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}";
    j["pact:matcher:type"] = "regex";
    j["regex"] = regex;
    if (!example.empty()) {
      if (pactffi_check_regex(regex, example.data())) {
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
    return j.dump();
  }

  std::string IncludesMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "include";
    j["value"] = value;
    return j.dump();
  }

  std::string NullMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "null";
    return j.dump();
  }

  std::string UrlMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "regex";
    std::string regex;
    if (pathFragments.empty()) {
      regex = ".*";
    } else {
      regex = ".*(";
      for (const auto &p : pathFragments) {
        regex += "\\/" + p->as_regex();
      }
      regex += ")$";
    }
    j["regex"] = regex;
    std::string example = basePath;
    if (basePath.empty()) {
      example = "http://localhost:8080";
    }
    for (const auto &p : pathFragments) {
      example += "/" + p->as_example();
    }

    // Temporary fix for inconsistancies between matchers and generators. Matchers use "value" attribute for
    // example values, while generators use "example"
    j["value"] = example;
    if (basePath.empty()) {
      j["example"] = example;
      j["pact:generator:type"] = "MockServerURL";
    }
    return j.dump();
  }

  std::string ArrayContainsMatcher::getJson() const {
    json j;
    j["pact:matcher:type"] = "arrayContains";

    json array = json::array();
    for (const auto &v : variants) {
      array.push_back(json::parse(v->getJson()));
    }
    j["variants"] = array;
    
    return j.dump();
  }
}
