/*
 * Private data for NodeShared.
 */

#ifndef AEROZEN_NODE_SHARED_PRIVATE_HPP_
#define AEROZEN_NODE_SHARED_PRIVATE_HPP_

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <zmq.hpp>

#include "aerozen/discovery.hpp"
#include "aerozen/handler_storage.hpp"
#include "aerozen/message_info.hpp"
#include "aerozen/publisher.hpp"
#include "aerozen/rep_handler.hpp"
#include "aerozen/req_handler.hpp"
#include "aerozen/subscription_handler.hpp"
#include "aerozen/topic_statistics.hpp"

namespace zenoh {
class Session;
}

namespace aerozen {

class NodeSharedPrivate
{
public:
    static constexpr int Timeout = 100;
    struct PublicationMetadata {
        uint64_t seq = 0;
        uint64_t stamp = 0;
    };

    struct PublishMsgDetails {
        MessageInfo info;
        std::unique_ptr<ProtoMsg> msgCopy;
        std::string publisherNodeUUID;
        std::vector<ISubscriptionHandlerPtr> localHandlers;
        std::vector<RawSubscriptionHandlerPtr> rawHandlers;
        size_t msgSize = 0;
        std::unique_ptr<char[]> sharedBuffer;
    };

    void PublishThread();
    void SecurityOnNewConnection();
    void SecurityInit();
    void AccessControlHandler();
    int NonNegativeEnvVar(const std::string& _envVar, int _defaultVal) const;

    bool exit = false;
    bool topicStatsEnabled = false;
    bool verbose = false;

    std::string gzImplementation = "zeromq";
    std::string hostAddr;
    std::string myAddress;
    std::string myRequesterAddress;
    std::string myReplierAddress;

    std::unique_ptr<MsgDiscovery> msgDiscovery;
    std::unique_ptr<SrvDiscovery> srvDiscovery;

    std::unique_ptr<zmq::socket_t> publisher;
    std::unique_ptr<zmq::socket_t> subscriber;
    std::unique_ptr<zmq::socket_t> requester;
    std::unique_ptr<zmq::socket_t> replier;
    std::unique_ptr<zmq::socket_t> responseReceiver;
    std::unique_ptr<zmq::context_t> context;

    std::thread pubThread;
    std::thread srvThread;
    std::thread accessControlThread;

    std::mutex pubThreadMutex;
    std::condition_variable signalNewPub;
    std::deque<std::unique_ptr<PublishMsgDetails>> pubQueue;

    std::mutex srvThreadMutex;
    std::condition_variable signalNewSrv;
    std::deque<ServicePublisher> srvQueue;

    std::unordered_map<std::string, uint64_t> topicPubSeq;
    std::map<std::string, TopicStatistics> topicStats;
    std::map<std::string, std::function<void(const TopicStatistics&)>>
        enabledTopicStatistics;

    std::unordered_map<std::string, std::unordered_set<std::string>>
        topicsSubscribed;

    HandlerStorage<IReqHandler> requests;
    HandlerStorage<IRepHandler> repliers;

#ifdef HAVE_ZENOH
    std::shared_ptr<zenoh::Session> session;
#endif
};

}  // namespace aerozen

#endif  // AEROZEN_NODE_SHARED_PRIVATE_HPP_
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

#ifndef AEROZEN_NODE_SHARED_PRIVATE_HPP_
#define AEROZEN_NODE_SHARED_PRIVATE_HPP_

#include <map>
#include <string>

#include "aerozen/config.hpp"
#include "aerozen/net_utils.hpp"

namespace aerozen {
//
/// \internal
/// \brief Private data for NodeOption class.
class NodeOptionsPrivate
{
    /// \brief Constructor.
public:
    NodeOptionsPrivate() = default;

    /// \brief Destructor.
public:
    virtual ~NodeOptionsPrivate() = default;

    /// \brief Namespace for this node.
public:
    std::string ns;

    /// \brief Partition for this node.
public:
    std::string partition = HostName() + ":" + UserName();

    /// \brief Table of remappings. The key is the original topic name and
    /// its value is the new topic name to be used instead.
public:
    std::map<std::string, std::string> topicsRemap;
};
}  // namespace aerozen

#endif  // AEROZEN_NODE_SHARED_PRIVATE_HPP_
