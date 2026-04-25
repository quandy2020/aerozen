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

#include <atomic>
#include <chrono>
#include <csignal>
#include <functional>
#include <thread>

#include "gtest/gtest.h"

#include "aerozen/proto/int32.pb.h"

#include "aerozen/node.hpp"
#include "aerozen/test/test_utils.hpp"
#include "aerozen/utils/environment.hpp"
#include "aerozen/wait_helpers.hpp"

/**
 * @brief WaitUntil: predicate immediately returns true near-instantly.
 */
TEST(WaitHelpersTest, WaitUntilImmediatelyTrue) {
    auto t1 = std::chrono::steady_clock::now();
    EXPECT_TRUE(WaitUntil([] { return true; }));
    auto t2 = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    EXPECT_LT(elapsed, 100);
}

//////////////////////////////////////////////////
/**
 * @brief WaitUntil: predicate never returns true; expect false after timeout.
 */
TEST(WaitHelpersTest, WaitUntilNeverTrue) {
    auto t1 = std::chrono::steady_clock::now();
    EXPECT_FALSE(
        WaitUntil([] { return false; }, std::chrono::milliseconds(100)));
    auto t2 = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    EXPECT_GE(elapsed, 90);
    EXPECT_LT(elapsed, 500);
}

/**
 * @brief WaitUntil: predicate becomes true mid-wait.
 */
TEST(WaitHelpersTest, WaitUntilBecomesTrueMidWait) {
    std::atomic<int> callCount{0};
    auto pred = [&]() -> bool { return ++callCount >= 5; };
    EXPECT_TRUE(WaitUntil(pred, std::chrono::milliseconds(5000),
                          std::chrono::milliseconds(10)));
    EXPECT_GE(callCount.load(), 5);
}

/**
 * @brief WaitUntil: zero timeout edge cases.
 */
TEST(WaitHelpersTest, WaitUntilZeroTimeout) {
    EXPECT_FALSE(WaitUntil([] { return false; }, std::chrono::milliseconds(0)));
    EXPECT_TRUE(WaitUntil([] { return true; }, std::chrono::milliseconds(0)));
}

/**
 * @brief WaitUntil: elapsed time is close to timeout on failure.
 */
TEST(WaitHelpersTest, WaitUntilElapsedCloseToTimeout) {
    auto t1 = std::chrono::steady_clock::now();
    EXPECT_FALSE(
        WaitUntil([] { return false; }, std::chrono::milliseconds(200)));
    auto t2 = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    EXPECT_GE(elapsed, 180);
    EXPECT_LT(elapsed, 1000);
}

/**
 * @brief WaitForService: service not advertised returns false.
 */
TEST(WaitHelpersTest, WaitForServiceNotAdvertised) {
    aerozen::utils::setenv("AEROZEN_PARTITION", testing::GetRandomNumber());
    transport::Node node;
    EXPECT_FALSE(
        WaitForService(node, "/nonexistent", std::chrono::milliseconds(200)));
}

/**
 * @brief WaitForService: service already advertised returns true.
 */
TEST(WaitHelpersTest, WaitForServiceAlreadyAdvertised) {
    aerozen::utils::setenv("AEROZEN_PARTITION", testing::GetRandomNumber());
    transport::Node node;

    std::function<bool(const aerozen::msgs::Int32&, aerozen::msgs::Int32&)> cb =
        [](const aerozen::msgs::Int32& _req,
           aerozen::msgs::Int32& _rep) -> bool {
        _rep.set_data(_req.data());
        return true;
    };
    EXPECT_TRUE(node.Advertise("/test_svc", cb));

    EXPECT_TRUE(
        WaitForService(node, "/test_svc", std::chrono::milliseconds(2000)));
}

/**
 * @brief WaitForTopic: topic already advertised returns true.
 */
TEST(WaitHelpersTest, WaitForTopicAlreadyAdvertised) {
    aerozen::utils::setenv("AEROZEN_PARTITION", testing::GetRandomNumber());
    transport::Node node;
    auto pub = node.Advertise<msgs::Int32>("/test_topic");
    EXPECT_TRUE(pub);

    EXPECT_TRUE(
        WaitForTopic(node, "/test_topic", std::chrono::milliseconds(2000)));
}

/**
 * @brief Spawn a thread blocked on WaitForShutdown(), send SIGINT, join.
 * @note Relies on the transport stack registering signal handlers as
 * documented for WaitForShutdown().
 */
TEST(WaitHelpersTest, WaitForShutdownSIGINT) {
    std::thread aThread([] { WaitForShutdown(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    raise(SIGINT);
    aThread.join();
}

/**
 * @brief Spawn a thread blocked on WaitForShutdown(), send SIGTERM, join.
 */
TEST(WaitHelpersTest, WaitForShutdownSIGTERM) {
    std::thread aThread([] { WaitForShutdown(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    raise(SIGTERM);
    aThread.join();
}

/**
 * @brief Stress re-entry into WaitForShutdown(): self-pipe persists across
 * calls; loop wait/signal to catch leftover bytes or handler ordering issues.
 */
TEST(WaitHelpersTest, WaitForShutdownReEntryStress) {
    for (int i = 0; i < 10; ++i) {
        std::thread aThread([] { WaitForShutdown(); });
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        raise(SIGINT);
        aThread.join();
    }
}
