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

#ifndef AEROZEN_PARAMETERS_UTILS_HPP_
#define AEROZEN_PARAMETERS_UTILS_HPP_

#include <optional>
#include <string>

#include <google/protobuf/any.pb.h>

namespace aerozen::parameters {

/**
 * @brief Get the aerozen message type from a protobuf message.
 * @param[in] _any Message to get the type.
 * @return A string with the aerozen protobuf type, or std::nullopt if it fails.
 */
std::optional<std::string> GetAerozenTypeFromAnyProto(
    const google::protobuf::Any& _any);
}  // namespace aerozen::parameters

#endif  // AEROZEN_PARAMETERS_UTILS_HPP_
