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

#include "aerozen/subscribe_options.hpp"
#include "aerozen/helpers.hpp"
#include "gtest/gtest.h"

using namespace aerozen;

TEST(SubscribeOptionsTest, copyConstructor) {
    SubscribeOptions opts1;
    opts1.SetMsgsPerSec(2u);
    EXPECT_EQ(opts1.MsgsPerSec(), 2u);
    SubscribeOptions opts2(opts1);
    EXPECT_EQ(opts2.MsgsPerSec(), opts1.MsgsPerSec());
}

TEST(SubscribeOptionsTest, accessors) {
    // MsgsPerSec.
    SubscribeOptions opts;
    EXPECT_EQ(opts.MsgsPerSec(), kUnthrottled);
    opts.SetMsgsPerSec(3u);
    EXPECT_EQ(opts.MsgsPerSec(), 3u);
}

TEST(SubscribeOptionsTest, throttled) {
    SubscribeOptions opts;
    EXPECT_FALSE(opts.Throttled());
    opts.SetMsgsPerSec(3u);
    EXPECT_TRUE(opts.Throttled());
}
