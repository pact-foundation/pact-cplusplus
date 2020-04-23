#include "consumer.h"

using namespace pact_mock_server_ffi;

namespace pact_consumer
{
  PactJsonBuilder::PactJsonBuilder(const PactJsonBuilder* parent) {
    this->parent = parent;
    this->obj = json::object();
  }
  
  PactJsonBuilder& PactJsonBuilder::eachLike(std::string name, void (*callback)(PactJsonBuilder&)) {
    return eachLike(name, 1, callback);
  }

  PactJsonBuilder& PactJsonBuilder::eachLike(std::string name, unsigned int examples, void (*callback)(PactJsonBuilder&)) {
    PactJsonBuilder builder(this);
    callback(builder);

    json array = json::array();
    for (unsigned int i = 0; i < examples; i++) {
      array.push_back(builder.get_json());
    }
    obj[name] = array;

    return *this;
  }

  /*
   * Value must match the given template
   * @param template Template to base the comparison on
   
  export function like(template: any) {
    return {
      "pact:matcher:type": "type",
      value: template,
    }
  }*/

  PactJsonBuilder& PactJsonBuilder::atLeastOneLike(std::string name, void (*callback)(PactJsonBuilder&)) {
    return atLeastOneLike(name, 1, callback);
  }

  PactJsonBuilder& PactJsonBuilder::atLeastOneLike(std::string name, unsigned int examples, void (*callback)(PactJsonBuilder&)) {
    PactJsonBuilder builder(this);
    callback(builder);

    json array = json::array();
    for (unsigned int i = 0; i < examples; i++) {
      array.push_back(builder.get_json());
    }
    json j;
    j["pact:matcher:type"] = "integer"; 
    j["value"] = array; 
    obj[name] = j;

    return *this;
  }

  /*
   * An array that has to have at least the required number of elements and each element must match the given template
   * @param template Template to base the comparison on
   * @param min Minimum number of elements required in the array
   * @param count Number of examples to generate, defaults to one
   
  export function atLeastLike(template: any, min: number, count?: number) {
    const elements = count || min
    if (count && count < min) {
      throw new Error(
        "atLeastLike has a minimum of " +
          min +
          " but " +
          count +
          " elements where requested." +
          " Make sure the count is greater than or equal to the min."
      )
    }

    return {
      min,
      "pact:matcher:type": "type",
      value: R.times(() => template, elements),
    }
  }

  
   * An array that has to have at most the required number of elements and each element must match the given template
   * @param template Template to base the comparison on
   * @param max Maximum number of elements required in the array
   * @param count Number of examples to generate, defaults to one
   
  export function atMostLike(template: any, max: number, count?: number) {
    const elements = count || 1
    if (count && count > max) {
      throw new Error(
        "atMostLike has a maximum of " +
          max +
          " but " +
          count +
          " elements where requested." +
          " Make sure the count is less than or equal to the max."
      )
    }

    return {
      max,
      "pact:matcher:type": "type",
      value: R.times(() => template, elements),
    }
  }

  
   * An array whose size is constrained to the minimum and maximum number of elements and each element must match the given template
   * @param template Template to base the comparison on
   * @param min Minimum number of elements required in the array
   * @param max Maximum number of elements required in the array
   * @param count Number of examples to generate, defaults to one
   
  export function constrainedArrayLike(
    template: any,
    min: number,
    max: number,
    count?: number
  ) {
    const elements = count || min
    if (count) {
      if (count < min) {
        throw new Error(
          "constrainedArrayLike has a minimum of " +
            min +
            " but " +
            count +
            " elements where requested." +
            " Make sure the count is greater than or equal to the min."
        )
      } else if (count > max) {
        throw new Error(
          "constrainedArrayLike has a maximum of " +
            max +
            " but " +
            count +
            " elements where requested." +
            " Make sure the count is less than or equal to the max."
        )
      }
    }

    return {
      min,
      max,
      "pact:matcher:type": "type",
      value: R.times(() => template, elements),
    }
  }*/

  PactJsonBuilder& PactJsonBuilder::boolean(std::string name, bool b) {
    json j;
    j["pact:matcher:type"] = "type"; 
    j["value"] = b; 
    obj[name] = j;
    return *this;
  }
  
  PactJsonBuilder& PactJsonBuilder::integer(std::string name, int example) {
    json j;
    j["pact:matcher:type"] = "integer"; 
    j["value"] = example; 
    obj[name] = j;
    return *this;
  }

  PactJsonBuilder& PactJsonBuilder::integer(std::string name) {
    json j;
    j["pact:generator:type"] = "RandomInt"; 
    j["pact:matcher:type"] = "integer"; 
    j["value"] = 101; 
    obj[name] = j;
    return *this;
  }

  /*
   * Value must be a decimal number (must be a number and have decimal places)
   * @param num Example value. If omitted a random value will be generated.
   
  export function decimal(num?: number) {
    if (num) {
      return {
        "pact:matcher:type": "decimal",
        value: num,
      }
    } else {
      return {
        "pact:generator:type": "RandomDecimal",
        "pact:matcher:type": "decimal",
        value: 12.34,
      }
    }
  }

  
   * Value must be a number
   * @param num Example value. If omitted a random integer value will be generated.
   
  export function number(num?: number) {
    if (num) {
      return {
        "pact:matcher:type": "number",
        value: num,
      }
    } else {
      return {
        "pact:generator:type": "RandomInt",
        "pact:matcher:type": "number",
        value: 1234,
      }
    }
  }*/

  PactJsonBuilder& PactJsonBuilder::string(std::string name, std::string example) {
    json j;
    j["pact:matcher:type"] = "type"; 
    j["value"] = example; 
    obj[name] = j;
    return *this;
  }

  /*
   * Value that must match the given regular expression
   * @param pattern Regular Expression to match
   * @param str Example value
   
  export function regex(pattern: string, str: string) {
    return {
      "pact:matcher:type": "regex",
      regex: pattern,
      value: str,
    }
  }

  
   * Value that must be equal to the example. This is mainly used to reset the matching rules which cascade.
   * @param value Example value
   
  export function equal(value: any) {
    return {
      "pact:matcher:type": "equality",
      value,
    }
  }*/

  PactJsonBuilder& PactJsonBuilder::datetime(std::string name, std::string format, std::string example) {
    json j;
    j["pact:generator:type"] = "DateTime"; 
    j["pact:matcher:type"] = "timestamp"; 
    j["timestamp"] = format;
    j["value"] = example;
    obj[name] = j;
    return *this;
  }

  PactJsonBuilder& PactJsonBuilder::datetime(std::string name, std::string format) {
    auto result = pact_mock_server_ffi::generate_datetime_string(format.data());
    if (result.tag == DateTimeResult::Tag::Ok) {
      std::string value = result.ok._0;
      pact_mock_server_ffi::free_string(result.ok._0);

      json j;
      j["pact:generator:type"] = "DateTime"; 
      j["pact:matcher:type"] = "timestamp"; 
      j["timestamp"] = format;
      j["value"] = value;
      obj[name] = j;
      return *this;
    } else {
      std::string error = result.failed._0;
      pact_mock_server_ffi::free_string(result.failed._0);
      throw error;
    }
  }

  /*
   * String value that must match the provided time format string.
   * @param format Time format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)
   * @param example Example value to use. If omitted a value using the current system time will be generated.
   
  export function time(format: string, example?: string) {
    return {
      "pact:generator:type": "Time",
      "pact:matcher:type": "time",
      time: format,
      value: example || PactNative.generate_datetime_string(format),
    }
  }

  
   * String value that must match the provided date format string.
   * @param format Date format string. See [Java SimpleDateFormat](https://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html)
   * @param example Example value to use. If omitted a value using the current system date will be generated.
   
  export function date(format: any, example?: string) {
    return {
      date: format,
      "pact:generator:type": "Date",
      "pact:matcher:type": "date",
      value: example || PactNative.generate_datetime_string(format),
    }
  }

  
   * Value that must include the example value as a substring.
   * @param value String value to include
   
  export function includes(value: string) {
    return {
      "pact:matcher:type": "include",
      value,
    }
  }

  
   * Value that must be null. This will only match the JSON Null value. For other content types, it will
   * match if the attribute is missing.
   
  export function nullValue() {
    return {
      "pact:matcher:type": "null",
    }
  }
  */

}
