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

#include "aerozen/utils/extra_test_macros.hpp"

GTEST_TEST(ExtraTestMacros,
           AEROZEN_UTILS_TEST_ENABLED_ONLY_ON_WINDOWS(OnlyWindowsOn)) {
#if defined __APPLE__
    ADD_FAILURE() << "Windows only test on Apple platform";
#elif defined __linux__
    ADD_FAILURE() << "Windows only test on Linux platform";
#endif
}

GTEST_TEST(ExtraTestMacros,
           AEROZEN_UTILS_TEST_DISABLED_ON_WINDOWS(OnlyWindowsOff)) {
#if defined AEROZEN_WINDOWS_DISABLED
    ADD_FAILURE() << "Ran on Windows platform";
#endif
}

GTEST_TEST(ExtraTestMacros, AEROZEN_UTILS_TEST_ENABLED_ONLY_ON_MAC(OnlyMacOn)) {
#if defined AEROZEN_WINDOWS_DISABLED
    ADD_FAILURE() << "Apple only test on Windows platform";
#elif defined __linux__
    ADD_FAILURE() << "Apple only test on Linux platform";
#endif
}

GTEST_TEST(ExtraTestMacros, AEROZEN_UTILS_TEST_DISABLED_ON_MAC(OnlyMacOff)) {
#if defined __APPLE__
    ADD_FAILURE() << "Ran on Apple platform";
#endif
}

GTEST_TEST(ExtraTestMacros,
           AEROZEN_UTILS_TEST_ENABLED_ONLY_ON_LINUX(OnlyLinuxOn)) {
#if defined AEROZEN_WINDOWS_DISABLED
    ADD_FAILURE() << "Linux only test on Windows platform";
#elif defined __APPLE__
    ADD_FAILURE() << "Linux only test on Apple platform";
#endif
}

GTEST_TEST(ExtraTestMacros,
           AEROZEN_UTILS_TEST_DISABLED_ON_LINUX(OnlyLinuxOff)) {
#if defined __linux__
    ADD_FAILURE() << "Ran on Linux platform";
#endif
}

GTEST_TEST(ExtraTestMacros,
           AEROZEN_UTILS_TEST_ENABLED_ONLY_ON_ARM32(OnlyArm32On)) {
#if defined __aarch64__ || defined _M_ARM64
    ADD_FAILURE() << "Ran on Arm64 arch";
#endif
}

GTEST_TEST(ExtraTestMacros,
           AEROZEN_UTILS_TEST_DISABLED_ON_ARM32(OnlyArm32Off)) {
#if defined __arm__ || defined _M_ARM
    ADD_FAILURE() << "Ran on Arm32 arch";
#endif
}

GTEST_TEST(ExtraTestMacros,
           AEROZEN_UTILS_TEST_ENABLED_ONLY_ON_ARM64(OnlyArm64On)) {
#if defined __arm__ || defined _M_ARM
    ADD_FAILURE() << "Ran on Arm32 arch";
#endif
}

GTEST_TEST(ExtraTestMacros,
           AEROZEN_UTILS_TEST_DISABLED_ON_ARM64(OnlyArm64Off)) {
#if defined __aarch64__ || defined _M_ARM64
    ADD_FAILURE() << "Ran on Arm64 arch";
#endif
}
