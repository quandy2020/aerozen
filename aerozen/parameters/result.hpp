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

#ifndef AEROZEN_PARAMETERS_RESULT_HPP_
#define AEROZEN_PARAMETERS_RESULT_HPP_

#include <ostream>
#include <string>

namespace aerozen::parameters {

/**
 * @brief Possible result types of the different parameters operations.
 */
enum class ParameterResultType {
    Success,
    AlreadyDeclared,
    InvalidType,
    NotDeclared,
    ClientTimeout,
    Unexpected,
};

/**
 * @brief The return type used in all falible parameters methods.
 */
class ParameterResult
{
public:
    /**
     * @brief Construct.
     * @param _resultType Type of result of the operation.
     */
    explicit ParameterResult(ParameterResultType _resultType);

    /**
     * @brief Construct.
     * @param _resultType Type of result of the operation.
     * @param _paramName Name of the related parameter.
     */
    ParameterResult(ParameterResultType _resultType,
                    const std::string& _paramName);

    /**
     * @brief Construct.
     * @param _resultType Type of result of the operation.
     * @param _paramName Name of the related parameter.
     * @param _paramType Type of the related parameter.
     */
    ParameterResult(ParameterResultType _resultType,
                    const std::string& _paramName,
                    const std::string& _paramType);

    /**
     * @brief  Return the result type.
     * @return The result type.
     */
    ParameterResultType ResultType() const;

    /**
     * @brief  Return the related parameter name.
     * @return The related parameter name.
     */
    const std::string& ParamName() const;

    /**
     * @brief  Return the related parameter type.
     * @return The related parameter type.
     */
    const std::string& ParamType() const;

    /**
     * @brief Coercion to bool type.
     * @return True if ParameterErrorType::Success, else False.
     */
    explicit operator bool() const;

private:
    ParameterResultType resultType;
    std::string paramName;
    std::string paramType;
};

/**
 * @brief Stream operator, for debug output.
 * @param[out] _out The output stream.
 * @param[in] _result The parameter result.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream&, const ParameterResult&);
}  // namespace aerozen::parameters

#endif  // AEROZEN_PARAMETERS_RESULT_HPP_
