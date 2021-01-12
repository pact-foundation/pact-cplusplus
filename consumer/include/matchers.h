#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <optional>
#include <boost/throw_exception.hpp>
#include <sstream>
#include <vector>

namespace pact_consumer::matchers {

  class IMatcher {
    public:
      using Ptr = std::shared_ptr<IMatcher>;
      virtual std::string getJson() const = 0;
      virtual std::string as_example() const { return ""; };
      virtual std::string as_regex() const { return as_example(); };
  };

  class ObjectMatcher : public IMatcher {
    public:
      ObjectMatcher(const std::unordered_map<std::string, IMatcher::Ptr>& i_fields) : fields { i_fields } {};

      virtual std::string getJson() const;

    private:
      std::unordered_map<std::string, IMatcher::Ptr> fields;
  };

  class IntegerMatcher : public IMatcher {
    public:
      IntegerMatcher() {};
      IntegerMatcher(long v) : value { v } {};
      IntegerMatcher(int v) : value { v } {};

      virtual std::string getJson() const;

    private:
      std::optional<long> value;
  };

  class DecimalMatcher : public IMatcher {
    public:
      DecimalMatcher() {};
      DecimalMatcher(float v) : value { v } {};
      DecimalMatcher(double v) : value { v } {};

      virtual std::string getJson() const;

    private:
      std::optional<double> value;
  };

  template<typename T>
  class TypeMatcher : public IMatcher {
    public:
      TypeMatcher(T v) : value { v } {};

      virtual std::string getJson() const;

    private:
      T value;
  };

  template<typename T>
  class NumberMatcher : public IMatcher {
    public:
      NumberMatcher() {};
      NumberMatcher(T v) : value { v } {};

      virtual std::string getJson() const;

    private:
      std::optional<T> value;
  };

  template<typename T>
  class EqualsMatcher : public IMatcher {
    public:
      EqualsMatcher(T v) : value { v } {};

      virtual std::string getJson() const;
      
      virtual std::string as_example() const { 
        std::ostringstream stringStream;
        stringStream << value;
        return stringStream.str(); 
      };

    private:
      T value;
  };

  class DateTimeMatcher : public IMatcher {
    public:
      DateTimeMatcher(const std::string& f) : format { f } {};
      DateTimeMatcher(const std::string& f, const std::string& e) : format { f }, example { e } {};

      virtual std::string getJson() const;

    private:
      std::string format, example;
  };

  class DateMatcher : public IMatcher {
    public:
      DateMatcher(const std::string& f) : format { f } {};
      DateMatcher(const std::string& f, const std::string& e) : format { f }, example { e } {};

      virtual std::string getJson() const;

    private:
      std::string format, example;
  };

  class TimeMatcher : public IMatcher {
    public:
      TimeMatcher(const std::string& f) : format { f } {};
      TimeMatcher(const std::string& f, const std::string& e) : format { f }, example { e } {};

      virtual std::string getJson() const;

    private:
      std::string format, example;
  };

  class RegexMatcher : public IMatcher {
    public:
      RegexMatcher(const std::string& r) : regex { r } {};
      RegexMatcher(const std::string& r, const std::string& e) : regex { r }, example { e } {};

      virtual std::string getJson() const;

      virtual std::string as_example() const { return example; };
      virtual std::string as_regex() const { return regex; };

    private:
      std::string regex, example;
  };

  class EachlikeMatcher : public IMatcher {
    public:
      EachlikeMatcher(IMatcher::Ptr t) : obj { t } {};
      EachlikeMatcher(int e, IMatcher::Ptr t) : obj { t }, examples { e } {};
      EachlikeMatcher(int e, int m, IMatcher::Ptr t) : obj { t }, examples { e }, min { m } {};
      EachlikeMatcher(int e, int m, int mx, IMatcher::Ptr t) : obj { t }, examples { e }, min { m }, max { mx } {};

      virtual std::string getJson() const;

    private:
      int examples = 1;
      std::optional<int> min;
      std::optional<int> max;
      IMatcher::Ptr obj;
  };

  class ArrayContainsMatcher : public IMatcher {
    public:
      ArrayContainsMatcher(const std::vector<IMatcher::Ptr>& i_variants) : variants { i_variants } {};

      virtual std::string getJson() const;

    private:
      std::vector<IMatcher::Ptr> variants;
  };

  class HexadecimalMatcher : public IMatcher {
    public:
      HexadecimalMatcher() {};
      HexadecimalMatcher(const std::string& hex) : example { hex } {};

      virtual std::string getJson() const;

    private:
      std::string example;
  };

  class IPAddressMatcher : public IMatcher {
    public:
      IPAddressMatcher() {};
      IPAddressMatcher(const std::string& address) : example { address } {};

      virtual std::string getJson() const;

    private:
      std::string example;
  };

  class UuidMatcher : public IMatcher {
    public:
      UuidMatcher() {};
      UuidMatcher(const std::string& uuid) : example { uuid } {};

      virtual std::string getJson() const;

    private:
      std::string example;
  };

  class IncludesMatcher : public IMatcher {
    public:
      IncludesMatcher(const std::string& v) : value { v } {};

      virtual std::string getJson() const;

    private:
      std::string value;
  };

  class NullMatcher : public IMatcher {
    public:
      virtual std::string getJson() const;
  };

  class UrlMatcher : public IMatcher {
    public:
      UrlMatcher(const std::string& b, const std::vector<IMatcher::Ptr>& f) : basePath { b }, pathFragments { f } {};

      virtual std::string getJson() const;

    private:
      std::string basePath;
      std::vector<IMatcher::Ptr> pathFragments;
  };

  /**
   * Represents a JSON object (map of string fields)
   */
  IMatcher::Ptr Object(const std::unordered_map<std::string, IMatcher::Ptr>& fields);

  /**
   * Matchers an integer value (must be a number and have no decimal places).
   */
  IMatcher::Ptr Integer(long value);

  /**
   * Matchers an integer value (must be a number and have no decimal places).
   */
  IMatcher::Ptr Integer(int value);

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
  IMatcher::Ptr Like(const std::string& value);
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
  IMatcher::Ptr DateTime(const std::string& format, const std::string& example);

  /**
   * String value that must match the provided datetime format string. Example values will be generated using the current system date and time.
   * @param format Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)
   */
  IMatcher::Ptr DateTime(const std::string& format);

  /**
   * String value that must match the provided date format string.
   * @param format Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)
   * @param example Example value to use
   */
  IMatcher::Ptr Date(const std::string& format, const std::string& example);

  /**
   * String value that must match the provided date format string. Example values will be generated using the current system date and time.
   * @param format Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)
   */
  IMatcher::Ptr Date(const std::string& format);

  /**
   * String value that must match the provided time format string.
   * @param format Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)
   * @param example Example value to use
   */
  IMatcher::Ptr Time(const std::string& format, const std::string& example);

  /**
   * String value that must match the provided time format string. Example values will be generated using the current system date and time.
   * @param format Datetime format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)
   */
  IMatcher::Ptr Time(const std::string& format);

  /**
   * String value that must match the regular expression
   */
  IMatcher::Ptr Matching(const std::string& regex, const std::string& example);

  /**
   * String value that must match the regular expression. Random values will be generated for the examples.
   */
  IMatcher::Ptr Matching(const std::string& regex);

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

  /**
   * Match a hexadecimal value
   * @param example Example value
   */
  IMatcher::Ptr HexValue(const std::string& example);

  /**
   * Match a hexadecimal value. Random examples will be generated.
   */
  IMatcher::Ptr HexValue();

  /**
   * Match an IP Address
   * @param example Example value
   */
  IMatcher::Ptr IPAddress(const std::string& example);

  /**
   * Match an IP Address. Will use 127.0.0.1 for examples.
   */
  IMatcher::Ptr IPAddress();

  /**
   * Match a numeric value. This will match any numeric type (integer or floating point).
   * @param value Example value
   */
  IMatcher::Ptr Numeric(int example);

  /**
   * Match a numeric value. This will match any numeric type (integer or floating point).
   * @param value Example value
   */
  IMatcher::Ptr Numeric(long example);

  /**
   * Match a numeric value. This will match any numeric type (integer or floating point).
   * @param value Example value
   */
  IMatcher::Ptr Numeric(float example);

  /**
   * Match a numeric value. This will match any numeric type (integer or floating point).
   * @param value Example value
   */
  IMatcher::Ptr Numeric(double example);

  /**
   * Match a numeric value. Random decimal values will be generated for examples.
   */
  IMatcher::Ptr Numeric();

  /**
   * Match a decimal value (number with atleast one digit after the decimal point)
   * @param example Example value
   */
  IMatcher::Ptr Decimal(float example);

  /**
   * Match a decimal value (number with atleast one digit after the decimal point)
   * @param example Example value
   */
  IMatcher::Ptr Decimal(double example);

  /**
   * Match a decimal value (number with atleast one digit after the decimal point). Random values will be generated
   * for examples.
   */
  IMatcher::Ptr Decimal();

  /**
   * Match a universally unique identifier (UUID)
   * @param example value to use for examples
   */
  IMatcher::Ptr Uuid(const std::string& example);

  /**
   * Match a universally unique identifier (UUID). Random values will be used for examples.
   */
  IMatcher::Ptr Uuid();

  /**
   * Array with maximum size and each element like the template object.
   * @param max The maximum size of the array
   */
  IMatcher::Ptr AtMostLike(int max, const IMatcher::Ptr obj);

  /**
   * Array with maximum size and each element like the template object.
   * @param max The maximum size of the array
   * @param examples Number of examples to generate.
   */
  IMatcher::Ptr AtMostLike(int max, int examples, const IMatcher::Ptr obj);

  /**
   * Array with minimum size and each element like the template object.
   * @param min The minimum size of the array
   */
  IMatcher::Ptr MinArrayLike(int min, const IMatcher::Ptr obj);

  /**
   * Array with minimum size and each element like the template object.
   * @param min The minimum size of the array
   * @param examples Number of examples to generate.
   */
  IMatcher::Ptr MinArrayLike(int min, int examples, const IMatcher::Ptr obj);

  /**
   * Array with minimum and maximum size and each element like the template object.
   * @param min The minimum size of the array
   * @param max The maximum size of the array
   */
  IMatcher::Ptr ConstrainedArrayLike(int min, int max, const IMatcher::Ptr obj);

  /**
   * Array with minimum and maximum size and each element like the template object.
   * @param min The minimum size of the array
   * @param max The maximum size of the array
   * @param examples number of examples to generate.
   */
  IMatcher::Ptr ConstrainedArrayLike(int min, int max, int examples, const IMatcher::Ptr obj);

  /**
   * Match by equality. This is mainly used to reset the cascading type matchers.
   * @param value Value to match to
   */
  IMatcher::Ptr EqualTo(int value);

  /**
   * Match by equality. This is mainly used to reset the cascading type matchers.
   * @param value Value to match to
   */
  IMatcher::Ptr EqualTo(long value);

  /**
   * Match by equality. This is mainly used to reset the cascading type matchers.
   * @param value Value to match to
   */
  IMatcher::Ptr EqualTo(float value);

  /**
   * Match by equality. This is mainly used to reset the cascading type matchers.
   * @param value Value to match to
   */
  IMatcher::Ptr EqualTo(double value);

  /**
   * Match by equality. This is mainly used to reset the cascading type matchers.
   * @param value Value to match to
   */
  IMatcher::Ptr EqualTo(const std::string& value);

  /**
   * Match by equality. This is mainly used to reset the cascading type matchers.
   * @param value Value to match to
   */
  IMatcher::Ptr EqualTo(const char *value);

  /**
   * Match by equality. This is mainly used to reset the cascading type matchers.
   * @param value Value to match to
   */
  IMatcher::Ptr EqualTo(bool value);

  /**
   * Matches if the string value contains the given value
   * @param value String value that must be present
   */
  IMatcher::Ptr IncludesStr(const std::string& value);

  /**
   * Matches a JSON null value
   */
  IMatcher::Ptr NullValue();

  /**
   * Matches a URL composed of a base path and a list of path fragments
   * @param basePath Base path of the URL
   * @param pathFragments list of path fragments, can be regular expressions. Only the Equals and Matching matchers will work.
   */
  IMatcher::Ptr Url(const std::string& basePath, const std::vector<IMatcher::Ptr>& pathFragments);

  /**
   * Matches a URL composed of a base path and a list of path fragments. The base URL from the mock server will be used.
   * @param pathFragments list of path fragments, can be regular expressions. Only the Equals and Matching matchers will work.
   */
  IMatcher::Ptr Url(const std::vector<IMatcher::Ptr>& pathFragments);

  /**
   * Matches the items in an array against a number of variants. Matching is successful if each variant 
   * occurs once in the array. Variants may be objects containing matching rules.
   * @param min The minimum size of the array
   * @param max The maximum size of the array
   * @param examples number of examples to generate.
   */
  IMatcher::Ptr ArrayContaining(const std::vector<IMatcher::Ptr>& variants);
}
