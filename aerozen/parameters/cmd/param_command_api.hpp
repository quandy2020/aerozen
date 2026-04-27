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

#ifndef AEROZEN_PARAMETERS_CMD_PARAM_COMMAND_API_HPP_
#define AEROZEN_PARAMETERS_CMD_PARAM_COMMAND_API_HPP_

/**
 * @brief External hook to get a list of available parameters.
 * @param[in] _ns Namespace of the parameter registry.
 */
extern "C" void CmdParametersList(const char* _ns);

/**
 * @brief External hook to dump a parameter.
 * @param[in] _ns Namespace of the parameter registry.
 * @param[in] _paramName Parameter name.
 */
extern "C" void CmdParameterGet(const char* _ns, const char* _paramName);

/**
 * @brief External hook to set a parameter.
 * @param[in] _ns Namespace of the parameter registry.
 * @param[in] _paramName Parameter name.
 * @param[in] _paramType Parameter protobuf type.
 * @param[in] _paramValue String representation of the parameter value.
 */
extern "C" void CmdParameterSet(const char* _ns, const char* _paramName,
                                const char* _paramType,
                                const char* _paramValue);

#endif  // AEROZEN_PARAMETERS_CMD_PARAM_COMMAND_API_HPP_
