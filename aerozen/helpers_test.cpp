/*
 * Basic tests for helpers.
 */

#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "aerozen/helpers.hpp"
#include "aerozen/test/test_utils.hpp"
#include "aerozen/utils/environment.hpp"

TEST(HelpersTest, Env) {
  std::string name = testing::GetRandomNumber();
  std::string value;
  EXPECT_FALSE(aerozen::env(name, value));

  ASSERT_TRUE(aerozen::utils::setenv(name, name));
  EXPECT_TRUE(aerozen::env(name, value));
  EXPECT_EQ(name, value);
}

TEST(HelpersTest, Split) {
  std::vector<std::string> parts = aerozen::split("a:b:c", ':');
  ASSERT_EQ(3u, parts.size());
  EXPECT_EQ("a", parts[0]);
  EXPECT_EQ("b", parts[1]);
  EXPECT_EQ("c", parts[2]);
}

TEST(HelpersTest, GetTransportImplementation) {
  std::string impl = aerozen::getTransportImplementation();
  EXPECT_FALSE(impl.empty());
}
