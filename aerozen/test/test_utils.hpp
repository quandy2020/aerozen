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

#ifndef AEROZEN_TEST_UTILS_HPP_
#define AEROZEN_TEST_UTILS_HPP_

#include <climits>
#include <memory>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

#include "gtest/gtest.h"

#include "aerozen/helpers.hpp"
#include "aerozen/utils/environment.hpp"
#include "aerozen/utils/subprocess.hpp"

namespace testing {
/**
 * @brief Get a random number based on an integer converted to string.
 * @return A random integer converted to string.
 */
inline std::string GetRandomNumber() {
    // Initialize random number generator.
    uint32_t seed = std::random_device{}();
    std::mt19937 randGenerator(seed);

    // Create a random number based on an integer converted to string.
    std::uniform_int_distribution<int32_t> d(0, INT_MAX);

    return std::to_string(d(randGenerator));
}

inline std::string getRandomNumber() {
    return GetRandomNumber();
}

/**
 * @brief An RAII wrapper for the aerozen::utils::Subprocess class that joins
 * the process at destruction.
 */
class SubprocessJoinWrapper
{
public:
    /**
     * @brief Constructor.
     * @param[in] _commandLine Vector of command line arguments.
     */
    explicit SubprocessJoinWrapper(const std::vector<std::string>& _commandLine)
        : proc(aerozen::utils::Subprocess(_commandLine)) {}

    // We don't want this to be copied.
    SubprocessJoinWrapper(const SubprocessJoinWrapper&) = delete;

    /**
     * @brief Join on the process at destruction. Note that if the child
     * process doesn't exit normally, this will block indefinitely, so this
     * should only be used for fixed duration sub-processes.
     */
    ~SubprocessJoinWrapper() {
        proc.Join();
    }

    /// \brief Underlying SubProcess object.
    aerozen::utils::Subprocess proc;
};

/**
 * @brief Base test fixture for two-process service call tests.
 * Subclasses override ReplierExecutable() to specify the replier binary.
 */
class TwoProcSrvCallFixture : public testing::Test
{
protected:
    virtual std::string ReplierExecutable() const = 0;

    void SetUp() override {
        aerozen::utils::env("AEROZEN_PARTITION", this->prevPartition);
        this->partition = testing::getRandomNumber();
        aerozen::utils::setenv("AEROZEN_PARTITION", this->partition);
        this->pi = std::make_unique<aerozen::utils::Subprocess>(
            std::vector<std::string>({ReplierExecutable(), this->partition}));
    }

    void TearDown() override {
        aerozen::utils::setenv("AEROZEN_PARTITION", this->prevPartition);
        this->pi->Terminate();
        this->pi->Join();
    }

private:
    std::string prevPartition;
    std::string partition;
    std::unique_ptr<aerozen::utils::Subprocess> pi;
};
}  // namespace testing

#define CHECK_UNSUPPORTED_IMPLEMENTATION(...)                   \
    if (std::unordered_set<std::string>({__VA_ARGS__})          \
            .count(aerozen::GetTransportImplementation()) != 0) \
        GTEST_SKIP() << "aerozen implementation '"              \
                     << aerozen::GetTransportImplementation()   \
                     << "' unsupported";

#endif  // AEROZEN_TEST_UTILS_HPP_
