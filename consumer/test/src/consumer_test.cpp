#include <gtest/gtest.h>
#include <consumer.h>

TEST(PactConsumerTest, ExampleContract) {
  EXPECT_EQ(pact_consumer::get_value(), 1);
}
