/*
 * Copyright (C) 2026 Aerozen Robotics
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

#ifndef AEROZEN_NET_UTILS_HPP_
#define AEROZEN_NET_UTILS_HPP_

#include <string>
#include <vector>

namespace aerozen {
/// \brief Determine if an IP is private.
/// Reference: https://github.com/ros/ros_comm/blob/hydro-devel/clients/
/// roscpp/src/libros/network.cpp
/// \param[in] _ip Input IP address.
/// \return true if the IP address is private.
bool isPrivateIP(const char* _ip);

/// \brief Determine if an IP is private.
/// \param[in] _hostname Hostname
/// \param[out] _ip IP associated to the input hostname.
/// \return 0 when success.
int hostnameToIp(char* _hostname, std::string& _ip);

/// \brief Determine IP or hostname.
/// Reference: https://github.com/ros/ros_comm/blob/hydro-devel/clients/
/// roscpp/src/libros/network.cpp
/// \return The IP or hostname of this host.
std::string determineHost();

/// \brief Determine the list of network interfaces for this machine.
/// Reference: https://github.com/ros/ros_comm/blob/hydro-devel/clients/
/// roscpp/src/libros/network.cpp
/// \return The list of network interfaces.
std::vector<std::string> determineInterfaces();

/// \brief Determine the computer's hostname.
/// \return The computer's hostname.
std::string hostname();

/// \brief Determine your login name.
/// \return Name used to gain access to the computer.
/// On linux and Mac only, if determination
/// of your login name fails then a string of the form "error-UUID"
/// is returned where UUID is a universally unique identifier.
std::string username();
}  // namespace aerozen

#endif  // AEROZEN_NET_UTILS_HPP_
