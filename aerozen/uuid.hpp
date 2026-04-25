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

#include <uuid/uuid.h>

#include <iostream>
#include <string>

using portable_uuid_t = uuid_t;

namespace aerozen {
/**
 * @class Uuid Uuid.hpp aerozen/uuid.hpp
 * @brief A portable class for representing a Universally Unique Identifier.
 */
class Uuid
{
public:
    /**
     * @brief Constructor.
     */
    Uuid();

    /**
     * @brief Destructor.
     */
    virtual ~Uuid();

    /**
     * @brief Return the string representation of the Uuid.
     * @return The UUID in string format.
     */
    std::string ToString() const;

    /**
     * @brief Stream insertion operator.
     * @param[out] _out The output stream.
     * @param[in] _uuid UUID to write to the stream.
     */
    friend std::ostream& operator<<(std::ostream& _out, const Uuid& _uuid) {
        _out << _uuid.ToString();
        return _out;
    }

private:
    /**
     * @brief Length of a UUID in string format.
     *
     * A UUID is a 16-octet number. In its string representation, every octet
     * is divided in two parts, and each part (4 bits) is represented as a
     * hexadecimal value. A UUID is also displayed in five groups separated
     * by hyphens, in the form 8-4-4-4-12 for a total of 36 characters.
     * To summarize: 36 octets + \0 = 37 octets.
     */
    static const int UuidStrLen = 37;

    /**
     * @brief Internal representation.
     */
    portable_uuid_t data;
};
}  // namespace aerozen

#endif  // AEROZEN_UUID_HPP_
