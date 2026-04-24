/*
 * Copyright (C) 2016 Open Source Robotics Foundation
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

#ifndef AEROZEN_SUBSCRIBE_OPTIONS_HPP_
#define AEROZEN_SUBSCRIBE_OPTIONS_HPP_

#include <cstdint>
#include <memory>

namespace aerozen {
  class SubscribeOptionsPrivate;

  /// \class SubscribeOptions subscribe_options.hpp
  /// aerozen/subscribe_options.hpp
  /// \brief A class to provide different options for a subscription.
  class SubscribeOptions
  {
   public:
    /// \brief Constructor.
    SubscribeOptions();

    /// \brief Copy constructor.
    /// \param[in] _otherSubscribeOpts SubscribeOptions to copy.
    SubscribeOptions(const SubscribeOptions& _otherSubscribeOpts);

    /// \brief Destructor.
    ~SubscribeOptions();

    /// \brief Whether the subscription has been throttled.
    /// \return true when the subscription is throttled or false otherwise.
    /// \sa SetMsgsPerSec
    /// \sa MsgsPerSec
    bool Throttled() const;

    /// \brief Set the maximum number of messages per second received per
    /// topic. Note that we calculate the minimum period of a message based
    /// on the msgs/sec rate. Any message received since the last subscription
    /// callback and the duration of the period will be discarded.
    /// \param[in] _newMsgsPerSec Maximum number of messages per second.
    void SetMsgsPerSec(uint64_t _newMsgsPerSec);

    /// \brief Get the maximum number of messages per seconds received per
    /// topic.
    /// \return The maximum number of messages per second.
    uint64_t MsgsPerSec() const;

    /// \brief Set the value to ignore local messages or not.
    /// \param[in] _ignore True when ignoring local messages
    /// or false otherwise.
    /// \sa IgnoreLocalMessages
    void SetIgnoreLocalMessages(bool _ignore);

    /// \brief Whether the local messages should be ignored.
    /// \return true when the local messages should be ignored or
    /// false otherwise.
    /// \sa SetIgnoreLocalMessages
    bool IgnoreLocalMessages() const;

   protected:
    /// \internal
    /// \brief Pointer to private data.
    std::unique_ptr<SubscribeOptionsPrivate> dataPtr;
  };
}  // namespace aerozen
#endif  // AEROZEN_SUBSCRIBE_OPTIONS_HPP_
