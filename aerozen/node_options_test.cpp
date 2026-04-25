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

#include "gtest/gtest.h"

#include <string>

#include "aerozen/net_utils.hpp"
#include "aerozen/node_options.hpp"
#include "aerozen/utils/environment.hpp"

using namespace aerozen;

/**
 * @brief Check that the partition is set.
 */
TEST(NodeOptionsTest, ignPartition) {
    // Set AEROZEN_PARTITION
    std::string aPartition = "customPartition";
    ASSERT_TRUE(aerozen::utils::setenv("AEROZEN_PARTITION", aPartition));

    aerozen::NodeOptions opts;
    EXPECT_EQ(opts.Partition(), aPartition);

    // A partition set by the user should overwrite AEROZEN_PARTITION.
    std::string userPartition = "userPartition";
    opts.SetPartition(userPartition);
    EXPECT_EQ(opts.Partition(), userPartition);

    // Copy constructor
    aerozen::NodeOptions opts2(opts);
    EXPECT_EQ(opts.Partition(), opts2.Partition());
    EXPECT_EQ(opts.NameSpace(), opts2.NameSpace());

    EXPECT_FALSE(opts2.SetPartition("/"));
}

TEST(NodeOptionsTest, accessors) {
    // Check the default values.
    aerozen::utils::unsetenv("AEROZEN_PARTITION");
    aerozen::NodeOptions opts;
    EXPECT_TRUE(opts.NameSpace().empty());
    auto defaultPartition = aerozen::hostname() + ":" + aerozen::username();
    EXPECT_EQ(opts.Partition(), defaultPartition);

    // NameSpace.
    std::string aNamespace = "validNamespace";
    EXPECT_FALSE(opts.SetNameSpace("invalid namespace"));
    EXPECT_EQ(opts.NameSpace(), "");
    EXPECT_TRUE(opts.SetNameSpace(aNamespace));
    EXPECT_EQ(opts.NameSpace(), aNamespace);

    // Partition.
    std::string aPartition = "validPartition";
    EXPECT_FALSE(opts.SetPartition("invalid partition"));
    EXPECT_EQ(opts.Partition(), defaultPartition);
    EXPECT_TRUE(opts.SetPartition(aPartition));
    EXPECT_EQ(opts.Partition(), aPartition);
}
