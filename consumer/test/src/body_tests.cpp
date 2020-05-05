#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <consumer.h>
#include <nlohmann/json.hpp>
  
using json = nlohmann::json;
using namespace testing;
using namespace pact_consumer;

TEST(PactJsonBuilderTest, EachLike) {
  PactJsonBuilder builder(nullptr);

  builder.eachLike("attr", [](auto obj) {
    (*obj).string("one", "one").integer("two", 2);
  });

  std::string json = builder.get_json().dump().data();
  EXPECT_EQ(json, "{\"attr\":[{\"one\":{\"pact:matcher:type\":\"type\",\"value\":\"one\"},\"two\":{\"pact:matcher:type\":\"integer\",\"value\":2}}]}");
}

TEST(PactJsonBuilderTest, EachLikeWithNumExamples) {
  PactJsonBuilder builder(nullptr);

  builder.eachLike("attr", 2, [](auto obj) {
    (*obj).integer("two", 2);
  });

  std::string json = builder.get_json().dump().data();
  EXPECT_EQ(json, "{\"attr\":[{\"two\":{\"pact:matcher:type\":\"integer\",\"value\":2}},{\"two\":{\"pact:matcher:type\":\"integer\",\"value\":2}}]}");
}

TEST(PactJsonBuilderTest, StringValue) {
  PactJsonBuilder builder(nullptr);

  builder
    .string("foo", "Foo")
    .string("bar");

  std::string json = builder.get_json().dump().data();
  EXPECT_EQ(json, "{\"bar\":{\"pact:generator:size\":20,\"pact:generator:type\":\"RandomString\",\"pact:matcher:type\":\"type\",\"value\":\"example string\"},\"foo\":{\"pact:matcher:type\":\"type\",\"value\":\"Foo\"}}");
}

TEST(PactJsonBuilderTest, NumberValue) {
  PactJsonBuilder builder(nullptr);

  builder
    .decimal("num1")
    .decimal("num2", 100.6)
    .integer("num3", 100)
    .integer("num4")
    .number("num5", 100)
    .number("num6", 100.6)
    .number("num7");

  std::string json = builder.get_json().dump().data();
  EXPECT_EQ(json, "{\"num1\":{\"pact:generator:type\":\"RandomDecimal\",\"pact:matcher:type\":\"decimal\",\"value\":12.34},\
\"num2\":{\"pact:matcher:type\":\"decimal\",\"value\":100.6},\
\"num3\":{\"pact:matcher:type\":\"integer\",\"value\":100},\
\"num4\":{\"pact:generator:type\":\"RandomInt\",\"pact:matcher:type\":\"integer\",\"value\":101},\
\"num5\":{\"pact:matcher:type\":\"number\",\"value\":100},\
\"num6\":{\"pact:matcher:type\":\"number\",\"value\":100.6},\
\"num7\":{\"pact:generator:type\":\"RandomInt\",\"pact:matcher:type\":\"number\",\"value\":1234}}");
}

TEST(PactJsonBuilderTest, RegexMatcher) {
  PactJsonBuilder builder(nullptr);

  builder
    .regex("foo", "[a-z][0-9]", "a1")
    .regex("bar", "\\d+");

  std::string json_str = builder.get_json().dump().data();
  json j = json::parse(json_str);
  EXPECT_EQ(j["foo"].dump(), "{\"pact:matcher:regex\":\"[a-z][0-9]\",\"pact:matcher:type\":\"regex\",\"value\":\"a1\"}");
  EXPECT_EQ(j["bar"]["pact:matcher:type"], "regex");
  EXPECT_THAT(j["bar"]["value"].get<std::string>(), MatchesRegex("[0-9]+"));
}

