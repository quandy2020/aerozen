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

#include <iostream>
#include <string>
#include "aerozen/proto/stringmsg.pb.h"
#include "aerozen/node.hpp"
#include "aerozen/wait_helpers.hpp"

/**
 * @brief Function called each time a topic update is received.
 */
void cb(const char *_data, const size_t _size,
        const aerozen::MessageInfo &_info)
{
  aerozen::msgs::StringMsg msg;
  msg.ParseFromArray(_data, _size);

  std::cout << "Msg length: " << _size << " bytes" << std::endl;
  std::cout << "Msg contents: " << msg.data() << std::endl;
}

/**
 * @brief Main function.
 */
int main(int argc, char **argv)
{
  aerozen::Node node;
  std::string topic = "/foo";

  // Subscribe to a topic by registering a callback.
  if (!node.SubscribeRaw(topic, cb))
  {
    std::cerr << "Error subscribing to topic [" << topic << "]" << std::endl;
    return -1;
  }

  // Zzzzzz.
  aerozen::WaitForShutdown();

  return 0;
}
