/*
 * Copyright (C) 2026 duyongquan <quandy2020@126.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <gtest/gtest.h>

#include "aerozen/utils/environment.hpp"

#include <string>

using namespace aerozen;

/////////////////////////////////////////////////
TEST(Environment, emptyENV) {
    aerozen::utils::clearenv();

    std::string var;
    EXPECT_FALSE(aerozen::utils::env("!!SHOULD_NOT_EXIST!!", var));
    EXPECT_TRUE(var.empty());
}

/////////////////////////////////////////////////
TEST(Environment, envSet) {
    aerozen::utils::clearenv();

    const auto key = "AEROZEN_ENV_SET";
    ASSERT_TRUE(aerozen::utils::setenv(key, "VALUE"));

    // Check set var
    {
        std::string value;
        EXPECT_TRUE(aerozen::utils::env(key, value));
        EXPECT_FALSE(value.empty());
        EXPECT_EQ("VALUE", value);
    }

    // Check set var with allowEmpty
    {
        std::string value;
        EXPECT_TRUE(aerozen::utils::env(key, value, true));
        EXPECT_FALSE(value.empty());
        EXPECT_EQ("VALUE", value);
    }

    // Check set var without allowEmpty
    {
        std::string value;
        EXPECT_TRUE(aerozen::utils::env(key, value, false));
        EXPECT_FALSE(value.empty());
        EXPECT_EQ("VALUE", value);
    }

    ASSERT_TRUE(aerozen::utils::unsetenv(key));
}

/////////////////////////////////////////////////
TEST(Environment, envUnset) {
    aerozen::utils::clearenv();

    const auto key = "AEROZEN_ENV_UNSET";
    ASSERT_TRUE(aerozen::utils::unsetenv(key));

    // Check unset var (default)
    {
        std::string value;
        EXPECT_FALSE(aerozen::utils::env(key, value));
        EXPECT_TRUE(value.empty());
    }

    // Check unset var with allowEmpty
    {
        std::string value;
        EXPECT_FALSE(aerozen::utils::env(key, value, true));
        EXPECT_TRUE(value.empty());
    }

    // Check unset var without allowEmpty
    {
        std::string value;
        EXPECT_FALSE(aerozen::utils::env(key, value, false));
        EXPECT_TRUE(value.empty());
    }
    ASSERT_TRUE(aerozen::utils::unsetenv(key));
}

/////////////////////////////////////////////////
TEST(Environment, envSetEmpty) {
    aerozen::utils::clearenv();

    const auto key = "AEROZEN_ENV_SET_EMPTY";

    ASSERT_TRUE(aerozen::utils::setenv(key, ""));
    ASSERT_FALSE(aerozen::utils::setenv("", ""));

    // Check set empty var (default)
    {
        std::string value;
        EXPECT_FALSE(aerozen::utils::env(key, value));
        EXPECT_TRUE(value.empty());
    }

    {
        // This will not warn and return true on Linux,
        // as empty environment variables are allowed.
        std::string value;
        EXPECT_TRUE(aerozen::utils::env(key, value, true));
        EXPECT_TRUE(value.empty());
    }

    // Check set empty var without allowEmpty
    {
        std::string value;
        EXPECT_FALSE(aerozen::utils::env(key, value, false));
        EXPECT_TRUE(value.empty());
    }
    ASSERT_TRUE(aerozen::utils::unsetenv(key));
}

/////////////////////////////////////////////////
TEST(Environment, envGetCollection) {
    aerozen::utils::clearenv();
    auto currentEnv = aerozen::utils::env();
    EXPECT_EQ(currentEnv.size(), 0);

    ASSERT_TRUE(aerozen::utils::setenv("AEROZEN_FOO_KEY", "AEROZEN_FOO_VAL"));
    ASSERT_TRUE(aerozen::utils::setenv("AEROZEN_BAR_KEY", "AEROZEN_BAR_VAL"));
    ASSERT_TRUE(aerozen::utils::setenv("AEROZEN_BAZ_KEY", "AEROZEN_BAZ_VAL"));

    currentEnv = aerozen::utils::env();
    EXPECT_EQ(currentEnv.size(), 3);

    EXPECT_EQ(currentEnv["AEROZEN_FOO_KEY"], "AEROZEN_FOO_VAL");
    EXPECT_EQ(currentEnv["AEROZEN_BAR_KEY"], "AEROZEN_BAR_VAL");
    EXPECT_EQ(currentEnv["AEROZEN_BAZ_KEY"], "AEROZEN_BAZ_VAL");
}

/////////////////////////////////////////////////
TEST(Environment, printenv) {
    aerozen::utils::clearenv();
    EXPECT_EQ(aerozen::utils::printenv(), "");

    ASSERT_TRUE(aerozen::utils::setenv("AEROZEN_FOO_KEY", "AEROZEN_FOO_VAL"));
    ASSERT_TRUE(aerozen::utils::setenv("AEROZEN_BAR_KEY", "AEROZEN_BAR_VAL"));
    ASSERT_TRUE(aerozen::utils::setenv("AEROZEN_BAZ_KEY", "AEROZEN_BAZ_VAL"));

    // Always returned in sorted order
    EXPECT_EQ(aerozen::utils::printenv(),
              "AEROZEN_BAR_KEY=AEROZEN_BAR_VAL\nAEROZEN_BAZ_KEY=AEROZEN_BAZ_"
              "VAL\nAEROZEN_FOO_KEY=AEROZEN_FOO_VAL\n");
}

/////////////////////////////////////////////////
TEST(Environment, envStringsToMap) {
    aerozen::utils::EnvironmentStrings strings;
    strings.emplace_back("AEROZEN_FOO_KEY=AEROZEN_FOO_VAL");
    strings.emplace_back("AEROZEN_BAR_KEY=AEROZEN_BAR_VAL");
    strings.emplace_back("AEROZEN_BAZ_KEY=AEROZEN_BAZ_VAL");
    strings.emplace_back("BAD_KEY");

    {
        auto envMap = aerozen::utils::envStringsToMap(strings);
        EXPECT_EQ(3u, envMap.size());
        EXPECT_EQ("AEROZEN_FOO_VAL", envMap["AEROZEN_FOO_KEY"]);
        EXPECT_EQ("AEROZEN_BAR_VAL", envMap["AEROZEN_BAR_KEY"]);
        EXPECT_EQ("AEROZEN_BAZ_VAL", envMap["AEROZEN_BAZ_KEY"]);
    }
}

/////////////////////////////////////////////////
TEST(Environment, envMapToStrings) {
    aerozen::utils::EnvironmentMap envMap;
    envMap.insert({{"AEROZEN_FOO_KEY"}, {"AEROZEN_FOO_VAL"}});
    envMap.insert({{"AEROZEN_BAR_KEY"}, {"AEROZEN_BAR_VAL"}});
    envMap.insert({{"AEROZEN_BAZ_KEY"}, {"AEROZEN_BAZ_VAL"}});

    {
        auto envStrings = aerozen::utils::envMapToStrings(envMap);

        EXPECT_EQ(3u, envStrings.size());
        EXPECT_EQ("AEROZEN_BAR_KEY=AEROZEN_BAR_VAL", envStrings[0]);
        EXPECT_EQ("AEROZEN_BAZ_KEY=AEROZEN_BAZ_VAL", envStrings[1]);
        EXPECT_EQ("AEROZEN_FOO_KEY=AEROZEN_FOO_VAL", envStrings[2]);
    }
}
