#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <boost/throw_exception.hpp>
#include <sstream>

using json = nlohmann::json;

namespace pact_consumer::matchers {

  class IMatcher {
    public:
      using Ptr = std::shared_ptr<IMatcher>;
      virtual json getJson() const = 0;
  };

  class ObjectMatcher : public IMatcher {
    public:
      ObjectMatcher(const std::unordered_map<std::string, IMatcher::Ptr> i_fields) : fields { i_fields } {};

      virtual json getJson() const;

    private:
      std::unordered_map<std::string, IMatcher::Ptr> fields;
  };

  class IntegerMatcher : public IMatcher {
    public:
      IntegerMatcher() {};
      IntegerMatcher(long v) : value { v } {};

      virtual json getJson() const;

    private:
      std::optional<long> value;
  };

  template<typename T>
  class TypeMatcher : public IMatcher {
    public:
      TypeMatcher(T v) : value { v } {};

      virtual json getJson() const;

    private:
      T value;
  };

  class DateTimeMatcher : public IMatcher {
    public:
      DateTimeMatcher(std::string f) : format { f } {};
      DateTimeMatcher(std::string f, std::string e) : format { f }, example { e } {};

      virtual json getJson() const;

    private:
      std::string format, example;
  };

  class RegexMatcher : public IMatcher {
    public:
      RegexMatcher(std::string r) : regex { r } {};
      RegexMatcher(std::string r, std::string e) : regex { r }, example { e } {};

      virtual json getJson() const;

    private:
      std::string regex, example;
  };

  class EachlikeMatcher : public IMatcher {
    public:
      EachlikeMatcher(IMatcher::Ptr t) : obj { t }, examples { 1 }, min { 0 } {};
      EachlikeMatcher(int e, IMatcher::Ptr t) : obj { t }, examples { e }, min { 0 } {};
      EachlikeMatcher(int e, int m, IMatcher::Ptr t) : obj { t }, examples { e }, min { m } {};

      virtual json getJson() const;

    private:
      int examples, min;
      IMatcher::Ptr obj;
  };

  /**
   * Represents a JSON object (map of string fields)
   */
  IMatcher::Ptr Object(const std::unordered_map<std::string, IMatcher::Ptr> fields);

  /**
   * Matchers an integer value (must be a number and have no decimal places).
   */
  IMatcher::Ptr Integer(long value);

  /**
   * Matchers an integer value (must be a number and have no decimal places). A random example value will be generated.
   */
  IMatcher::Ptr Integer();

  /**
   * Matchers value based on the value type
   */
  IMatcher::Ptr Like(long value);
  /**
   * Matchers value based on the value type
   */
  IMatcher::Ptr Like(int value);
  /**
   * Matchers value based on the value type
   */
  IMatcher::Ptr Like(double value);
  /**
   * Matchers value based on the value type
   */
  IMatcher::Ptr Like(std::string value);
  /**
   * Matchers value based on the value type
   */
  IMatcher::Ptr Like(const char *value);
  /**
   * Matchers value based on the value type
   */
  IMatcher::Ptr Like(bool value);

  /**
   * String value that must match the provided datetime format string.
   * @param format Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)
   * @param example Example value to use
   */
  IMatcher::Ptr DateTime(std::string format, std::string example);

  /**
   * String value that must match the provided datetime format string. Example values will be generated using the current system date and time.
   * @param format Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)
   */
  IMatcher::Ptr DateTime(std::string format);

  /**
   * String value that must match the regular expression
   */
  IMatcher::Ptr Matching(std::string regex, std::string example);

  /**
   * String value that must match the regular expression. Random values will be generated for the examples.
   */
  IMatcher::Ptr Matching(std::string regex);

  /**
   * Array of values where each item in the array must match the provided template.
   */
  IMatcher::Ptr EachLike(const IMatcher::Ptr obj);

  /**
   * Array of values where each item in the array must match the provided template.
   * @param examples The number of example values to generate
   */
  IMatcher::Ptr EachLike(int examples, const IMatcher::Ptr obj);

  /**
   * Array of values that must have at least one value and each item in the array must match the provided template.
   */
  IMatcher::Ptr AtLeastOneLike(const IMatcher::Ptr obj);

  /**
   * Array of values that must have at least one value and each item in the array must match the provided template.
   * @param examples The number of example values to generate
   */
  IMatcher::Ptr AtLeastOneLike(int examples, const IMatcher::Ptr obj);

}
