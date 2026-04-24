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

#include "aerozen/helpers.hpp"
#include "aerozen/subscribe_options.hpp"
#include "gtest/gtest.h"

using namespace aerozen;

TEST(SubscribeOptionsTest, DefaultValues)
{
  SubscribeOptions opts;

  EXPECT_EQ(kUnthrottled, opts.MsgsPerSec());
  EXPECT_FALSE(opts.Throttled());
  EXPECT_FALSE(opts.IgnoreLocalMessages());
}

TEST(SubscribeOptionsTest, SetMsgsPerSec)
{
  SubscribeOptions opts;
  opts.SetMsgsPerSec(42u);

  EXPECT_EQ(42u, opts.MsgsPerSec());
  EXPECT_TRUE(opts.Throttled());
}

TEST(SubscribeOptionsTest, DisableThrottling)
{
  SubscribeOptions opts;
  opts.SetMsgsPerSec(10u);
  EXPECT_TRUE(opts.Throttled());

  opts.SetMsgsPerSec(kUnthrottled);
  EXPECT_EQ(kUnthrottled, opts.MsgsPerSec());
  EXPECT_FALSE(opts.Throttled());
}

TEST(SubscribeOptionsTest, IgnoreLocalMessages)
{
  SubscribeOptions opts;
  opts.SetIgnoreLocalMessages(true);
  EXPECT_TRUE(opts.IgnoreLocalMessages());

  opts.SetIgnoreLocalMessages(false);
  EXPECT_FALSE(opts.IgnoreLocalMessages());
}

TEST(SubscribeOptionsTest, CopyConstructor)
{
  SubscribeOptions opts;
  opts.SetMsgsPerSec(100u);
  opts.SetIgnoreLocalMessages(true);

  SubscribeOptions copied(opts);
  EXPECT_EQ(100u, copied.MsgsPerSec());
  EXPECT_TRUE(copied.Throttled());
  EXPECT_TRUE(copied.IgnoreLocalMessages());
}
