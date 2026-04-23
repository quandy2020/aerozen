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

// A portable wrapper for a UUID value.
class Uuid {
 public:
  Uuid();
  ~Uuid() = default;

  Uuid(const Uuid&) = default;
  Uuid& operator=(const Uuid&) = default;
  Uuid(Uuid&&) = default;
  Uuid& operator=(Uuid&&) = default;

  // Returns the canonical UUID string (xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx).
  std::string ToString() const;

  friend std::ostream& operator<<(std::ostream& out, const Uuid& uuid) {
    out << uuid.ToString();
    return out;
  }

 private:
  // UUID string length including null terminator.
  static constexpr int kUuidStringLength = 37;

  portable_uuid_t data_;
};
}  // namespace aerozen

#endif  // AEROZEN_UUID_HPP_
