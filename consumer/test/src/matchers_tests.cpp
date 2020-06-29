#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <matchers.h>

using namespace pact_consumer::matchers;

TEST(UrlMatcher, WithEmptyPath) { 
  auto matcher = Url("http://localhost/tasks", {});
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"regex\",\"regex\":\".*\",\"value\":\"http://localhost/tasks\"}");
}

TEST(UrlMatcher, WithStringInPath) { 
  auto matcher = Url("http://localhost/tasks", { EqualTo("test") });
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"regex\",\"regex\":\".*\\\\/test\",\"value\":\"http://localhost/tasks/test\"}");
}

TEST(UrlMatcher, WithRegexInPath) { 
  auto matcher = Url("http://localhost/tasks", { Matching("\\d+", "100") });
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"regex\",\"regex\":\".*\\\\/\\\\d+\",\"value\":\"http://localhost/tasks/100\"}");
}


TEST(UrlMatcher, WithStringAndRegexInPath) { 
  auto matcher = Url("http://localhost/tasks", { EqualTo("1001"), Matching("\\d+", "200") });
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"regex\",\"regex\":\".*\\\\/1001\\\\/\\\\d+\",\"value\":\"http://localhost/tasks/1001/200\"}");
}

TEST(ConstrainedArrayLike, WithNoExamples) { 
  auto matcher = ConstrainedArrayLike(2, 4, Object({
    { "id", Integer(3) }
  }));
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"max\":4,\"min\":2,\"pact:matcher:type\":\"type\",\"value\":[{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}},{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}}]}");
}

TEST(ConstrainedArrayLike, WithExamples) { 
  auto matcher = ConstrainedArrayLike(2, 4, 3, Object({
    { "id", Integer(3) }
  }));
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"max\":4,\"min\":2,\"pact:matcher:type\":\"type\",\"value\":[{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}},{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}},{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}}]}");
}
