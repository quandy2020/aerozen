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

#include <cctype>
#include <sstream>
#include <string>

#include "aerozen/uuid.hpp"
#include "gtest/gtest.h"

namespace aerozen {

TEST(UuidTest, ToStringFormatAndUniqueness) {
  Uuid uuid1;
  Uuid uuid2;

  // Two UUIDs should always be different.
  EXPECT_NE(uuid1.ToString(), uuid2.ToString());

  std::ostringstream output;
  output << uuid1;
  const std::string uuid_string = output.str();

  ASSERT_EQ(uuid_string.size(), 36u);

  for (size_t i = 0; i < 8; ++i) {
    EXPECT_NE(std::isxdigit(static_cast<unsigned char>(uuid_string[i])), 0);
  }
  EXPECT_EQ(uuid_string[8], '-');

  for (size_t i = 9; i < 13; ++i) {
    EXPECT_NE(std::isxdigit(static_cast<unsigned char>(uuid_string[i])), 0);
  }
  EXPECT_EQ(uuid_string[13], '-');

  for (size_t i = 14; i < 18; ++i) {
    EXPECT_NE(std::isxdigit(static_cast<unsigned char>(uuid_string[i])), 0);
  }
  EXPECT_EQ(uuid_string[18], '-');

  for (size_t i = 19; i < 23; ++i) {
    EXPECT_NE(std::isxdigit(static_cast<unsigned char>(uuid_string[i])), 0);
  }
  EXPECT_EQ(uuid_string[23], '-');

  for (size_t i = 24; i < 36; ++i) {
    EXPECT_NE(std::isxdigit(static_cast<unsigned char>(uuid_string[i])), 0);
  }
}

}  // namespace aerozen
