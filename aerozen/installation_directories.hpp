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

#ifndef AEROZEN_INSTALLATION_DIRECTORIES_HPP_
#define AEROZEN_INSTALLATION_DIRECTORIES_HPP_

#include <string>

#include "aerozen/config.hpp"

namespace aerozen {
/// \brief getInstallPrefix return the install prefix of the library
/// i.e. CMAKE_INSTALL_PREFIX unless the library has been moved
std::string GetInstallPrefix();
}  // namespace aerozen

#endif
