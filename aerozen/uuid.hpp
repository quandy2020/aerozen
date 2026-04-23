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

#ifndef AEROZEN_UUID_HPP_
#define AEROZEN_UUID_HPP_

#include <ostream>
#include <string>

#include <uuid/uuid.h>

namespace aerozen {

using portable_uuid_t = uuid_t;

/**
 * @brief A portable wrapper for a UUID value.
 */
class Uuid {
 public:
  /**
   * @brief Constructs a UUID with a generated value.
   */
  Uuid();

  /**
   * @brief Default destructor.
   */
  ~Uuid() = default;

  Uuid(const Uuid&) = default;
  Uuid& operator=(const Uuid&) = default;
  Uuid(Uuid&&) = default;
  Uuid& operator=(Uuid&&) = default;

  /**
   * @brief Returns the canonical UUID string.
   *
   * The format is `xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx`.
   *
   * @return Canonical UUID string representation.
   */
  std::string ToString() const;

  friend std::ostream& operator<<(std::ostream& out, const Uuid& uuid) {
    out << uuid.ToString();
    return out;
  }

 private:
  /**
   * @brief UUID string length including null terminator.
   */
  static constexpr int kUuidStringLength = 37;

  portable_uuid_t data_;
};
}  // namespace aerozen

#endif  // AEROZEN_UUID_HPP_
