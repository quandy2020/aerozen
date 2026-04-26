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

#include <cstring>
#include <map>
#include <memory>

#include "aerozen/ciface.hpp"
#include "aerozen/node.hpp"
#include "aerozen/subscribe_options.hpp"

/// \brief A wrapper to store a transport node and its publishers.
struct TransportNode {
    /// \brief Pointer to the node.
    std::unique_ptr<aerozen::Node> nodePtr;

    /// \brief All publishers of this node.
    std::map<std::string, aerozen::Node::Publisher> publishers;
};

/////////////////////////////////////////////////
TransportNode* TransportNodeCreate(const char* _partition) {
    TransportNode* transportNode = new TransportNode();
    aerozen::NodeOptions opts;
    if (_partition)
        opts.SetPartition(_partition);
    transportNode->nodePtr = std::make_unique<aerozen::Node>(opts);
    return transportNode;
}

/////////////////////////////////////////////////
void TransportNodeDestroy(TransportNode** _node) {
    if (*_node) {
        delete *_node;
        *_node = nullptr;
    }
}

/////////////////////////////////////////////////
int TransportAdvertise(TransportNode* _node, const char* _topic,
                       const char* _msgType) {
    if (!_node)
        return 1;

    // Create a publisher if one does not exist.
    if (_node->publishers.find(_topic) == _node->publishers.end())
        _node->publishers[_topic] = _node->nodePtr->Advertise(_topic, _msgType);

    return 0;
}

/////////////////////////////////////////////////
int TransportPublishRaw(TransportNode* _node, const char* _topic,
                        const void* _data, size_t _size, const char* _msgType) {
    if (!_node)
        return 1;

    // Create a publisher if one does not exist.
    if (TransportAdvertise(_node, _topic, _msgType) == 0) {
        // Publish the message using explicit size to handle binary data with
        // embedded null bytes.
        return _node->publishers[_topic].PublishRaw(
                   std::string(reinterpret_cast<const char*>(_data), _size),
                   _msgType)
                   ? 0
                   : 1;
    }

    return 1;
}

int TransportPublish(TransportNode* _node, const char* _topic,
                     const void* _data, const char* _msgType) {
    return TransportPublishRaw(
        _node, _topic, _data, std::strlen(reinterpret_cast<const char*>(_data)),
        _msgType);
}

int TransportSubscribe(TransportNode* _node, const char* _topic,
                       void (*_callback)(const char*, size_t, const char*,
                                         void*),
                       void* _userData) {
    if (!_node)
        return 1;

    return _node->nodePtr->SubscribeRaw(
               _topic,
               [_callback, _userData](
                   const char* _msg, const size_t _size,
                   const aerozen::MessageInfo& _info) -> void {
                   _callback(_msg, _size, _info.Type().c_str(), _userData);
               })
               ? 0
               : 1;
}

int TransportSubscribeOptions(TransportNode* _node, const char* _topic,
                              SubscribeOpts _opts,
                              void (*_callback)(const char*, size_t,
                                                const char*, void*),
                              void* _userData) {
    if (!_node)
        return 1;

    aerozen::SubscribeOptions opts;
    opts.SetMsgsPerSec(_opts.msgsPerSec);

    return _node->nodePtr->SubscribeRaw(
               _topic,
               [_callback, _userData](
                   const char* _msg, const size_t _size,
                   const aerozen::MessageInfo& _info) -> void {
                   _callback(_msg, _size, _info.Type().c_str(), _userData);
               },
               aerozen::kGenericMessageType, opts)
               ? 0
               : 1;
}

int TransportSubscribeNonConst(TransportNode* _node, char* _topic,
                               void (*_callback)(char*, size_t, char*, void*),
                               void* _userData) {
    if (!_node)
        return 1;

    return _node->nodePtr->SubscribeRaw(
               _topic,
               [_callback, _userData](
                   const char* _msg, const size_t _size,
                   const aerozen::MessageInfo& _info) -> void {
                   _callback(const_cast<char*>(_msg), _size,
                             const_cast<char*>(_info.Type().c_str()),
                             _userData);
               })
               ? 0
               : 1;
}

int TransportUnsubscribe(TransportNode* _node, const char* _topic) {
    if (!_node)
        return 1;

    return _node->nodePtr->Unsubscribe(_topic) ? 0 : 1;
}

void TransportWaitForShutdown() {
    aerozen::WaitForShutdown();
}
