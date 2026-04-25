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

#ifndef AEROZEN_CIFACE_HPP_
#define AEROZEN_CIFACE_HPP_

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct SubscribeOpts {
    // cppcheck-suppress unusedStructMember
    unsigned int msgsPerSec;
} SubscribeOpts;

/// \brief A transport node.
typedef struct TransportNode TransportNode;

/// \brief Create a transport node.
/// \param[in] _partition Optional name of the partition to use.
/// Use nullptr to use the default value, which is specified via the
/// GZ_PARTITION environment variable.
/// \return A pointer to a new transport node. Do not manually delete this
/// pointer, instead use TransportNodeDestroy.
TransportNode* TransportNodeCreate(const char* _partition);

/// \brief Destroy a transport node.
/// \param[in, out] _node The transport node to destroy.
void TransportNodeDestroy(TransportNode** _node);

/// \brief Advertise a topic.
/// \param[in] _node Pointer to a node.
/// \param[in] _topic Topic on which to publish the message.
/// \param[in] _msgType Name of the message type.
/// \return 0 on success.
int TransportAdvertise(TransportNode* _node, const char* _topic,
                       const char* _msgType);

/// \brief Publishes a message on a topic.
/// \param[in] _node Pointer to a node.
/// \param[in] _topic Topic on which to publish the message.
/// \param[in] _data Byte array of serialized data to publish.
/// \param[in] _size Size of the data in bytes.
/// \param[in] _msgType Name of the message type.
/// \return 0 on success.
int TransportPublishRaw(TransportNode* _node, const char* _topic,
                        const void* _data, size_t _size, const char* _msgType);

/// \brief Publishes a message on a topic.
/// \param[in] _node Pointer to a node.
/// \param[in] _topic Topic on which to publish the message.
/// \param[in] _data Byte array of serialized data to publish.
/// \param[in] _msgType Name of the message type.
/// \return 0 on success.
/// \deprecated See gzTransportPublishRaw
int TransportPublish(TransportNode* _node, const char* _topic,
                     const void* _data, const char* _msgType);

/// \brief Subscribe to a topic, and register a callback.
/// \param[in] _node Pointer to a node.
/// \param[in] _topic Name of the topic.
/// \param[in] _callback The function to call when a message is received.
/// \param[in] _userData Arbitrary user data pointer.
/// \return 0 on success.
int gzTransportSubscribe(TransportNode* _node, const char* _topic,
                         void (*_callback)(const char*, size_t, const char*,
                                           void*),
                         void* _userData);

/// \brief Subscribe to a topic, and register a callback.
/// \param[in] _node Pointer to a node.
/// \param[in] _topic Name of the topic.
/// \param[in] _opts Subscriber options.
/// \param[in] _callback The function to call when a message is received.
/// \param[in] _userData Arbitrary user data pointer.
/// \return 0 on success.
int TransportSubscribeOptions(TransportNode* _node, const char* _topic,
                              SubscribeOpts _opts,
                              void (*_callback)(const char*, size_t,
                                                const char*, void*),
                              void* _userData);

/// \brief Subscribe to a topic, and register a callback.
/// \param[in] _node Pointer to a node.
/// \param[in] _topic Name of the topic.
/// \param[in] _callback The function to call when a message is received.
/// \param[in] _userData Arbitrary user data pointer.
/// \return 0 on success.
int gzTransportSubscribeNonConst(TransportNode* _node, char* _topic,
                                 void (*_callback)(char*, size_t, char*, void*),
                                 void* _userData);

/// \brief Unsubscribe from a topic.
/// \param[in] _node Pointer to a node.
/// \param[in] _topic Name of the topic.
/// \return 0 on success.
int gzTransportUnsubscribe(TransportNode* _node, const char* _topic);

/// \brief Block the current thread until a SIGINT or SIGTERM is received.
/// Note that this function registers a signal handler. Do not use this
/// function if you want to manage yourself SIGINT/SIGTERM.
void TransportWaitForShutdown();

#ifdef __cplusplus
}
#endif

#endif  // AEROZEN_CIFACE_HPP_
