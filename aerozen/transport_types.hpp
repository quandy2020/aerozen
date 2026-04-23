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

#ifndef AEROZEN_TRANSPORT_TYPES_HPP_
#define AEROZEN_TRANSPORT_TYPES_HPP_

#include <google/protobuf/message.h>

#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "aerozen/publisher.hpp"

namespace zenoh {
  class Sample;
}  // namespace zenoh

namespace aerozen {

class IRepHandler;
class IReqHandler;
class ISubscriptionHandler;
class RawSubscriptionHandler;
class MessageInfo;

/**
 * @def Addresses_M
 * @brief Map that stores all generic publishers.
 *
 * The keys are process UUIDs of nodes. For each UUID key, the value contains
 * the list of publishers advertising the topic within the same process UUID.
 *
 * @tparam T Publisher type stored in the map value list.
 */
template <typename T>
using Addresses_M = std::map<std::string, std::vector<T>>;

/**
 * @def MsgAddresses_M
 * @brief Specialized `Addresses_M` map for message publishers.
 */
using MsgAddresses_M = Addresses_M<MessagePublisher>;

/**
 * @def SrvAddresses_M
 * @brief Specialized `Addresses_M` map for service publishers.
 */
using SrvAddresses_M = Addresses_M<ServicePublisher>;

/**
 * @def ProtoMsg
 * @brief An abbreviated protobuf message type.
 */
using ProtoMsg = google::protobuf::Message;

/**
 * @def ProtoMsgPtr
 * @brief Shared pointer to any protobuf message.
 */
using ProtoMsgPtr = std::shared_ptr<ProtoMsg>;

/**
 * @def ISubscriptionHandlerPtr
 * @brief Shared pointer to `ISubscriptionHandler`.
 */
using ISubscriptionHandlerPtr = std::shared_ptr<ISubscriptionHandler>;

/**
 * @def RawSubscriptionHandlerPtr
 * @brief Shared pointer to `RawSubscriptionHandler`.
 */
using RawSubscriptionHandlerPtr = std::shared_ptr<RawSubscriptionHandler>;

/**
 * @def ISubscriptionHandler_M
 * @brief Map for subscription handlers of a topic.
 *
 * Each node can have its own subscription handler. The node id is used as the
 * key, and a pointer to a generic subscription handler is the value.
 */
using ISubscriptionHandler_M =
  std::map<std::string, ISubscriptionHandlerPtr>;

/**
 * @def RawSubscriptionHandler_M
 * @brief Map for raw subscription handlers of a topic.
 *
 * Each node can have its own raw subscription handler. The node id is used as
 * the key, and a pointer to a raw subscription handler is the value.
 */
using RawSubscriptionHandler_M =
  std::map<std::string, RawSubscriptionHandlerPtr>;

/**
 * @def IRepHandlerPtr
 * @brief Shared pointer to `IRepHandler`.
 */
using IRepHandlerPtr = std::shared_ptr<IRepHandler>;

/**
 * @def IReqHandlerPtr
 * @brief Shared pointer to `IReqHandler`.
 */
using IReqHandlerPtr = std::shared_ptr<IReqHandler>;

/**
 * @def IReqHandler_M
 * @brief Map for service request handlers of a topic.
 *
 * Each node can have its own request handler. The node id is used as the key.
 * The value is another map where the key is the request UUID and the value is
 * a pointer to a generic request handler.
 */
using IReqHandler_M =
  std::map<std::string, std::map<std::string, IReqHandlerPtr>>;

/**
 * @def DiscoveryCallback
 * @brief Callback type for discovery connections and disconnections.
 *
 * The callback provides publisher information advertising a topic.
 * Example: `void onDiscoveryResponse(const MessagePublisher &_publisher)`.
 *
 * @tparam T Publisher type used by the callback.
 */
template <typename T>
using DiscoveryCallback = std::function<void(const T &_publisher)>;

/**
 * @def MsgDiscoveryCallback
 * @brief Specialized discovery callback for a message publisher.
 */
using MsgDiscoveryCallback =
  std::function<void(const MessagePublisher &_publisher)>;

/**
 * @def SrvDiscoveryCallback
 * @brief Specialized discovery callback for a service publisher.
 */
using SrvDiscoveryCallback =
  std::function<void(const ServicePublisher &_publisher)>;

/**
 * @def LivelinessCallback
 * @brief Callback for Zenoh liveliness events.
 */
using LivelinessCallback =
  std::function<void(const zenoh::Sample &_sample)>;

/**
 * @def MsgCallback
 * @brief User callback used for receiving messages.
 *
 * @tparam T Message type delivered to the callback.
 * @param[in] _msg Protobuf message containing the topic update.
 * @param[in] _info Message information (e.g., topic name).
 */
template <typename T>
using MsgCallback =
  std::function<void(const T &_msg, const MessageInfo &_info)>;

/**
 * @def RawCallback
 * @brief User callback used for receiving raw message data.
 *
 * @param[in] _msgData String of a serialized protobuf message.
 * @param[in] _size Number of bytes in the serialized message data string.
 * @param[in] _info Message information.
 */
using RawCallback =
  std::function<void(const char *_msgData, const size_t _size,
                     const MessageInfo &_info)>;

/**
 * @def Timestamp
 * @brief Used to evaluate the validity of a discovery entry.
 */
using Timestamp = std::chrono::steady_clock::time_point;

/**
 * @def DeallocFunc
 * @brief Used when passing data to be published using ZMQ.
 *
 * @param[in] _data The buffer containing the message to be published.
 * @param[in] _hint This parameter can be used when a custom allocator is
 * used. For example, if a chunk was allocated through an allocator object, it
 * can be deallocated through the same object.
 *
 *   void my_free(void *data, void *hint)
 *   {
 *     (static_cast<allocator_t *>(hint))->free(data);
 *   }
 *
 * @ref http://zeromq.org/blog:zero-copy
 */
using DeallocFunc = void(void *_data, void *_hint);

/**
 * @brief The string type used for generic messages.
 */
constexpr char kGenericMessageType[] = "google.protobuf.Message";

/**
 * @brief The high water mark of the receive message buffer.
 *
 * @sa NodeShared::RcvHwm
 */
constexpr int kDefaultRcvHwm = 1000;

/**
 * @brief The high water mark of the send message buffer.
 *
 * @sa NodeShared::SndHwm
 */
constexpr int kDefaultSndHwm = 1000;
}  // namespace aerozen

#endif  // AEROZEN_TRANSPORT_TYPES_HPP_
