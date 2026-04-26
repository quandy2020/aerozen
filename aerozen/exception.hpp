/*
 * Copyright (C) 2026 Open Source Robotics Foundation
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

#ifndef AEROZEN_EXCEPTION_HPP_
#define AEROZEN_EXCEPTION_HPP_

#include <stdexcept>
#include <string>

namespace aerozen {
/**
 * @class Exception Exception.hpp aerozen/exception.hpp
 * @brief Exception class for aerozen errors.
 */
class Exception : public std::runtime_error
{
public:
    /**
     * @brief Inherit constructors from std::runtime_error.
     */
    using std::runtime_error::runtime_error;
};
}  // namespace aerozen
#endif  // AEROZEN_EXCEPTION_HPP_