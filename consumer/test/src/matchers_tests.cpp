#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <matchers.h>

using namespace pact_consumer::matchers;

TEST(UrlMatcher, WithEmptyPath) { 
  auto matcher = Url("http://localhost/tasks", {});
  std::string json = matcher->getJson().dump().data();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"regex\",\"regex\":\".*\",\"value\":\"http://localhost/tasks\"}");
}

TEST(UrlMatcher, WithStringInPath) { 
  auto matcher = Url("http://localhost/tasks", { EqualTo("test") });
  std::string json = matcher->getJson().dump().data();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"regex\",\"regex\":\".*\\\\/test\",\"value\":\"http://localhost/tasks/test\"}");
}

TEST(UrlMatcher, WithRegexInPath) { 
  auto matcher = Url("http://localhost/tasks", { Matching("\\d+", "100") });
  std::string json = matcher->getJson().dump().data();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"regex\",\"regex\":\".*\\\\/\\\\d+\",\"value\":\"http://localhost/tasks/100\"}");
}


TEST(UrlMatcher, WithStringAndRegexInPath) { 
  auto matcher = Url("http://localhost/tasks", { EqualTo("1001"), Matching("\\d+", "200") });
  std::string json = matcher->getJson().dump().data();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"regex\",\"regex\":\".*\\\\/1001\\\\/\\\\d+\",\"value\":\"http://localhost/tasks/1001/200\"}");
}
