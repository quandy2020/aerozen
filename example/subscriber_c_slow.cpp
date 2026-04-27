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
  const char *userData;

  if (_userData)
    userData = static_cast<const char *>(_userData);
  else
    userData = "null";

  printf("UseData[%s] Msg length: %zu bytes\n", userData, _size);
  printf("UseData[%s] Msg type: %s\n", userData, _msgType);
  printf("UseData[%s] Msg contents: %s\n", userData, msg.data().c_str());
}

/**
 * @brief Main function.
 */
int main(int argc, char **argv)
{
  // Create a transport node.
  TransportNode *node = TransportNodeCreate(nullptr);

  const char *topic = "/foo";

  SubscribeOpts opts;
  opts.msgsPerSec = 1;

  // Subscribe to a topic by registering a callback.
  if (TransportSubscribeOptions(node, topic, opts, cb, nullptr) != 0)
  {
    printf("Error subscribing to topic %s.\n", topic);
    return -1;
  }

  // Zzzzzz.
  TransportWaitForShutdown();
  TransportNodeDestroy(&node);

  return 0;
}
