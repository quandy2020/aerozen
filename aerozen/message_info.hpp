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

#ifndef AEROZEN_MESSAGE_INFO_HPP_
#define AEROZEN_MESSAGE_INFO_HPP_

#include <memory>
#include <string>

namespace aerozen {

class MessageInfoPrivate;

/**
 * @brief Provides information about a received message.
 */
class MessageInfo
{
public:
    /**
     * @brief Default constructor.
     */
    MessageInfo();

    /**
     * @brief Explicit copy constructor.
     * @param[in] _other Instance to copy data from.
     */
    MessageInfo(const MessageInfo& _other);

    /**
     * @brief Move constructor.
     * @param[in] _other Instance data is moved from.
     */
    MessageInfo(MessageInfo&& _other);  // NOLINT

    /**
     * @brief Destructor.
     */
    ~MessageInfo();

    /**
     * @brief Get the topic name associated to the message.
     * @return The topic name.
     */
    const std::string& Topic() const;

    /**
     * @brief Set the topic name associated to the message.
     * @param[in] _topic Topic name.
     */
    void SetTopic(const std::string& _topic);

    /**
     * @brief Get the name of the message type.
     * @return The message type name.
     */
    const std::string& Type() const;

    /**
     * @brief Set the name of the message type.
     * @param[in] _type Type name to set.
     */
    void SetType(const std::string& _type);

    /**
     * @brief Get the name of the partition.
     * @return The partition name.
     */
    const std::string& Partition() const;

    /**
     * @brief Set the partition of the topic where the message was published.
     * @param[in] _partition Partition name.
     */
    void SetPartition(const std::string& _partition);

    /**
     * @brief Set both topic and partition from a single string.
     * @param[in] _fullyQualifiedName Topic string with partition information.
     * @return True if topic and partition were set.
     * @sa TopicUtils::FullyQualifiedName
     */
    bool SetTopicAndPartition(const std::string& _fullyQualifiedName);

    /**
     * @brief Whether the message comes from a node within this process.
     * @return True when intra-process, false otherwise.
     */
    bool IntraProcess() const;

    /**
     * @brief Set whether this message is intra-process.
     * @param[in] _value Intra-process value.
     */
    void SetIntraProcess(bool _value);

protected:
    /**
     * @internal
     * @brief Pointer to private data.
     */
    std::unique_ptr<MessageInfoPrivate> dataPtr;
};
}  // namespace aerozen
#endif  // AEROZEN_MESSAGE_INFO_HPP_
