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
#include <stdio.h>
#include "aerozen/proto/stringmsg.pb.h"
#include "aerozen/ciface.hpp"

/**
 * @brief Function called each time a topic update is received.
 */
void cb(const char *_data, const size_t _size, const char *_msgType,
        void *_userData)
{
  aerozen::msgs::StringMsg msg;
  msg.ParseFromArray(_data, _size);
  const char *partition;

  if (_userData)
    partition = static_cast<const char *>(_userData);
  else
    partition = "null";

  printf("Partition[%s] Msg length: %zu bytes\n", partition, _size);
  printf("Partition[%s] Msg type: %s\n", partition, _msgType);
  printf("Partition[%s] Msg contents: %s\n", partition, msg.data().c_str());
}

/**
 * @brief Main function.
 */
int main(int argc, char **argv)
{
  const char *partName = "red";
  // Create a transport node.
  TransportNode *node = TransportNodeCreate(nullptr);
  TransportNode *nodeRed = TransportNodeCreate(partName);

  const char *topic = "/foo";

  // Subscribe to a topic by registering a callback.
  if (TransportSubscribe(node, topic, cb, nullptr) != 0)
  {
    printf("Error subscribing to topic %s.\n", topic);
    return -1;
  }

  // Subscribe to a topic by registering a callback.
  if (TransportSubscribe(nodeRed, topic, cb,
      const_cast<char*>(partName)) != 0)
  {
    printf("Error subscribing to topic %s.\n", topic);
    return -1;
  }

  // Zzzzzz.
  TransportWaitForShutdown();
  TransportNodeDestroy(&node);
  TransportNodeDestroy(&nodeRed);

  return 0;
}
