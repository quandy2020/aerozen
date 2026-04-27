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

#ifndef AEROZEN_PARAMETERS_INTERFACE_HPP_
#define AEROZEN_PARAMETERS_INTERFACE_HPP_

#include <functional>
#include <memory>
#include <string>
#include <variant>

#include <google/protobuf/message.h>

#include "aerozen/parameters/result.hpp"
#include "aerozen/proto/parameter_declarations.pb.h"

namespace aerozen::parameters {

/// \brief Common interface, implemented by ParametersRegistry
///   (local updates) and by ParametersClients (remote requests).
class ParametersInterface
{
    /// Default virtual destructor.
public:
    virtual ~ParametersInterface() = default;

    /// \brief Declare a new parameter.
    /// \param[in] _parameterName Name of the parameter to be declared.
    /// \param[in] _msg Protobuf message to be used as the initial
    ///   parameter value.
    /// \return A ParameterResult return code, can return error types:
    /// - ParameterResultType::AlreadyDeclared if the parameter was already
    ///   declared.
    /// - ParameterResultType::InvalidType if the parameter type is not
    ///   valid.
    virtual ParameterResult DeclareParameter(
        const std::string& _parameterName,
        const google::protobuf::Message& _msg) = 0;

    /// \brief Request the value of a parameter.
    /// \param[in] _parameterName Name of the parameter to be requested.
    /// \param[out] _parameter Output were the parameter value will be set.
    /// \return A ParameterResult return code, can return error types:
    /// - ParameterResultType::NotDeclared if the parameter was not
    ///   declared.
    /// - ParameterResultType::InvalidType if the parameter type was
    ///   invalid.
    /// - ParameterResultType::Unexpected, if an unexpected error happened.
    virtual ParameterResult Parameter(
        const std::string& _parameterName,
        google::protobuf::Message& _parameter) const = 0;

    /// \brief Request the value of a parameter.
    ///   Similar to the other overload, but it allocates a message of the
    ///   right type.
    ///
    /// \param[in] _parameterName Name of the parameter to be requested.
    /// \param[out] _parameter Output were the parameter value will be set.
    /// \return A ParameterResult return code, can return error types:
    /// - ParameterResultType::NotDeclared if the parameter was not
    ///   declared.
    /// - ParameterResultType::Unexpected, if an unexpected error happened.
    virtual ParameterResult Parameter(
        const std::string& _parameterName,
        std::unique_ptr<google::protobuf::Message>& _parameter) const = 0;

    /// \brief Set the value of a parameter.
    /// \param[in] _parameterName Name of the parameter to be set.
    /// \param[in] _msg Protobuf message to be used as the parameter value.
    /// \return A ParameterResult return code, can return error types:
    /// - ParameterResultType::NotDeclared if the parameter was not
    ///   declared.
    /// - ParameterResultType::InvalidType if the parameter type was
    ///   invalid.
    virtual ParameterResult SetParameter(
        const std::string& _parameterName,
        const google::protobuf::Message& _msg) = 0;

    /// \brief List all existing parameters.
    /// \return The name and types of existing parameters.
    virtual aerozen::msgs::ParameterDeclarations ListParameters() const = 0;
};
}  // namespace aerozen::parameters

#endif  // AEROZEN_PARAMETERS_INTERFACE_HPP_
