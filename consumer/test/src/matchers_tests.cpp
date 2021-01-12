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
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"regex\",\"regex\":\".*(\\\\/test)$\",\"value\":\"http://localhost/tasks/test\"}");
}

TEST(UrlMatcher, WithRegexInPath) { 
  auto matcher = Url("http://localhost/tasks", { Matching("\\d+", "100") });
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"regex\",\"regex\":\".*(\\\\/\\\\d+)$\",\"value\":\"http://localhost/tasks/100\"}");
}


TEST(UrlMatcher, WithStringAndRegexInPath) { 
  auto matcher = Url("http://localhost/tasks", { EqualTo("1001"), Matching("\\d+", "200") });
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"regex\",\"regex\":\".*(\\\\/1001\\\\/\\\\d+)$\",\"value\":\"http://localhost/tasks/1001/200\"}");
}

TEST(UrlMatcher, WithNoBasePath) { 
  auto matcher = Url({ EqualTo("1001"), Matching("\\d+", "200") });
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"example\":\"http://localhost:8080/1001/200\",\"pact:generator:type\":\"MockServerURL\",\"pact:matcher:type\":\"regex\",\"regex\":\".*(\\\\/1001\\\\/\\\\d+)$\",\"value\":\"http://localhost:8080/1001/200\"}");
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

TEST(ConstrainedArrayLike, WithNumberOfExamplesLessThanMin) { 
  auto matcher = ConstrainedArrayLike(2, 4, 1, Object({
    { "id", Integer(3) }
  }));
  ASSERT_THROW(matcher->getJson(), std::runtime_error);
}

TEST(ConstrainedArrayLike, WithNumberOfExamplesGreaterThanMax) { 
  auto matcher = ConstrainedArrayLike(2, 4, 5, Object({
    { "id", Integer(3) }
  }));
  ASSERT_THROW(matcher->getJson(), std::runtime_error);
}

TEST(ConstrainedArrayLike, WithMinGreaterThanMax) { 
  auto matcher = ConstrainedArrayLike(4, 2, 2, Object({
    { "id", Integer(3) }
  }));
  ASSERT_THROW(matcher->getJson(), std::runtime_error);
}

TEST(EachlikeMatcher, WithNoExamples) {
  auto matcher = EachLike(Object({
    { "id", Integer(3) }
  }));
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"type\",\"value\":[{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}}]}");
}

TEST(EachlikeMatcher, WithNumberOfExamples) { 
  auto matcher = EachLike(2, Object({
    { "id", Integer(3) }
  }));
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"type\",\"value\":[{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}},{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}}]}");
}

TEST(AtLeastOneLike, WithNoExamples) {
  auto matcher = AtLeastOneLike(Object({
    { "id", Integer(3) }
  }));
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"min\":1,\"pact:matcher:type\":\"type\",\"value\":[{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}}]}");
}

TEST(AtLeastOneLike, WithNumberOfExamples) { 
  auto matcher = AtLeastOneLike(2, Object({
    { "id", Integer(3) }
  }));
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"min\":1,\"pact:matcher:type\":\"type\",\"value\":[{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}},{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}}]}");
}

TEST(AtMostLike, WithNoExamples) {
  auto matcher = AtMostLike(2, Object({
    { "id", Integer(3) }
  }));
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"max\":2,\"pact:matcher:type\":\"type\",\"value\":[{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}}]}");
}

TEST(AtMostLike, WithNumberOfExamples) { 
  auto matcher = AtMostLike(4, 2, Object({
    { "id", Integer(3) }
  }));
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"max\":4,\"pact:matcher:type\":\"type\",\"value\":[{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}},{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}}]}");
}

TEST(AtMostLike, WithNumberOfExamplesGreaterThanMax) { 
  auto matcher = AtMostLike(2, 4, Object({
    { "id", Integer(3) }
  }));
  ASSERT_THROW(matcher->getJson(), std::runtime_error);
}

TEST(MinArrayLike, WithNoExamples) {
  auto matcher = MinArrayLike(2, Object({
    { "id", Integer(3) }
  }));
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"min\":2,\"pact:matcher:type\":\"type\",\"value\":[{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}},{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}}]}");
}

TEST(MinArrayLike, WithNumberOfExamples) { 
  auto matcher = MinArrayLike(2, 3, Object({
    { "id", Integer(3) }
  }));
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"min\":2,\"pact:matcher:type\":\"type\",\"value\":[{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}},{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}},{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}}]}");
}

TEST(MinArrayLike, WithNumberOfExamplesLessThanMin) { 
  auto matcher = MinArrayLike(4, 2, Object({
    { "id", Integer(3) }
  }));
  ASSERT_THROW(matcher->getJson(), std::runtime_error);
}

TEST(ArrayContaining, WithTwoVariants) { 
  auto matcher = ArrayContaining({ Object({ { "id", Integer(3) } }), Matching("\\d+", "100") });
  std::string json = matcher->getJson();
  EXPECT_EQ(json, "{\"pact:matcher:type\":\"arrayContains\",\"variants\":[{\"id\":{\"pact:matcher:type\":\"integer\",\"value\":3}},{\"pact:matcher:type\":\"regex\",\"regex\":\"\\\\d+\",\"value\":\"100\"}]}");
}
