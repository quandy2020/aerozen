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

#ifndef AEROZEN_NODE_PRIVATE_HPP_
#define AEROZEN_NODE_PRIVATE_HPP_

#include <string>
#include <unordered_set>

#include "aerozen/net_utils.hpp"
#include "aerozen/node.hpp"
#include "aerozen/node_options.hpp"
#include "aerozen/node_shared.hpp"

namespace aerozen {
class NodeShared;

/**
 * @brief Private data for Node class.
 */
class NodePrivate
{
    /**
     * @brief Constructor.
     */
public:
    NodePrivate() = default;

    /**
     * @brief Destructor.
     */
    virtual ~NodePrivate() = default;

    /**
     * @brief The list of service calls advertised by this node.
     */
    std::unordered_set<std::string> srvsAdvertised;

    /**
     * @brief Node UUID. This ID is unique for each node.
     */
    std::string nUuid;

    /**
     * @brief Pointer to the object shared between all the nodes within the
     * same process.
     */
    NodeShared* shared = NodeShared::Instance();

    /**
     * @brief Partition for this node.
     */
    std::string partition = HostName() + ":" + UserName();

    /**
     * @brief Default namespace for this node.
     */
    std::string ns;

    /**
     * @brief Custom options for this node.
     */
    NodeOptions options;

    /**
     * @brief Statistics publisher.
     */
    Node::Publisher statPub;
};
}  // namespace aerozen
#endif  // AEROZEN_NODE_PRIVATE_HPP_
