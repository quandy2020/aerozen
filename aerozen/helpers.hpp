/*
 * Copyright (C) 2014 Open Source Robotics Foundation
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

#ifndef AEROZEN_HELPERS_HPP_
#define AEROZEN_HELPERS_HPP_

#include <zmq.hpp>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

namespace aerozen {
//
/// \brief Constant used when not interested in throttling.
static const uint64_t kUnthrottled = std::numeric_limits<uint64_t>::max();

/// \brief Find the environment variable '_name' and return its value.
/// \param[in] _name Name of the environment variable.
/// \param[out] _value Value if the variable was found.
/// \return True if the variable was found or false otherwise.
bool env(const std::string& _name, std::string& _value);

/// \brief split at a one character delimiter to get a vector of something
/// \param[in] _orig The string to split
/// \param[in] _delim a character to split the string at
/// \returns vector of split pieces of the string excluding the delimiter
std::vector<std::string> split(const std::string& _orig, char _delim);

/// \brief Portable function to get the id of the current process.
/// \returns id of current process
unsigned int getProcessId();

/// \brief Get the name of the underlying transport implementation
/// \returns Name of the transport implementation, e.g. "zeromq", "zenoh"
std::string getTransportImplementation();

// Use safer functions on Windows
#define aerozen_strcat std::strcat
#define aerozen_strcpy std::strcpy
#define aerozen_sprintf std::sprintf
#define aerozen_strdup strdup
}  // namespace aerozen

#endif  // AEROZEN_HELPERS_HPP_
