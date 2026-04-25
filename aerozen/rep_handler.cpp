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

#include <memory>
#include <string>

#include "aerozen/config.hpp"
#include "aerozen/rep_handler.hpp"
#include "aerozen/topic_utils.hpp"
#include "aerozen/uuid.hpp"

#ifdef HAVE_ZENOH
#include <zenoh.hxx>
#endif

namespace aerozen {

/**
 * @brief Private data for IRepHandler class.
 */
class IRepHandlerPrivate
{
public:
    /**
     * @brief Default constructor.
     */
    IRepHandlerPrivate(const std::string& _pUuid, const std::string& _nUuid)
        : pUuid(_pUuid), nUuid(_nUuid), hUuid(Uuid().ToString()) {}

    /**
     * @brief Destructor.
     */
    virtual ~IRepHandlerPrivate() = default;

    /**
     * @brief Process UUID.
     */
    std::string pUuid;

    /**
     * @brief Node UUID.
     */
    std::string nUuid;

    /**
     * @brief Handler UUID.
     */
    std::string hUuid;

#ifdef HAVE_ZENOH
    /**
     * @brief Zenoh queriable to receive requests.
     */
    std::unique_ptr<zenoh::Queryable<void>> zQueryable;

    /**
     * @brief The liveliness token.
     */
    std::unique_ptr<zenoh::LivelinessToken> zToken;
#endif
};

/**
 * //////////////////////////////////////////////
 */
IRepHandler::IRepHandler(const std::string& _pUuid, const std::string& _nUuid)
    : dataPtr(new IRepHandlerPrivate(_pUuid, _nUuid)) {}

/**
 * //////////////////////////////////////////////
 */
IRepHandler::~IRepHandler() {}

/**
 * //////////////////////////////////////////////
 */
std::string IRepHandler::HandlerUuid() const {
    return this->dataPtr->hUuid;
}

#ifdef HAVE_ZENOH
/**
 * //////////////////////////////////////////////
 */
void IRepHandler::CreateZenohQueriable(std::shared_ptr<zenoh::Session> _session,
                                       const std::string& _service) {
    auto onQuery = [this, _service](const zenoh::Query& _query) {
        std::string output;
        std::string input = "";
        if (_query.get_payload())
            input = _query.get_payload()->get().as_string();

        if (this->RunCallback(input, output))
            _query.reply(_service, output);
    };

    auto onDropQueryable = []() {};

    zenoh::Session::QueryableOptions opts;
    this->dataPtr->zQueryable =
        std::make_unique<zenoh::Queryable<void>>(_session->declare_queryable(
            _service, onQuery, onDropQueryable, std::move(opts)));

    std::string token = TopicUtils::CreateLivelinessToken(
        _service, this->dataPtr->pUuid, this->dataPtr->nUuid, "SS",
        this->ReqTypeName(), this->RepTypeName());

    if (token.empty())
        return;

    this->dataPtr->zToken = std::make_unique<zenoh::LivelinessToken>(
        _session->liveliness_declare_token(token));
}
#endif
}  // namespace aerozen
