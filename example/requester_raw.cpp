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
#include "aerozen/proto/stringmsg.pb.h"
#include "aerozen/node.hpp"

/**
 * @brief Main function.
 */
int main(int argc, char **argv)
{
  // Create a transport node.
  aerozen::Node node;

  // Prepare the input parameters.
  aerozen::msgs::StringMsg req;
  req.set_data("HELLO");

  bool result;
  unsigned int timeout = 5000;

  std::string reqStr, repStr;
  req.SerializeToString(&reqStr);

  // Request the "/echo" service.
  bool executed = node.RequestRaw("/echo", reqStr, "aerozen.msgs.StringMsg",
      "aerozen.msgs.StringMsg", timeout, repStr, result);

  if (executed)
  {
    if (result)
    {
      aerozen::msgs::StringMsg rep;
      rep.ParseFromString(repStr);
      std::cout << "Response: [" << rep.data() << "]" << std::endl;
    }
    else
      std::cout << "Service call failed" << std::endl;
  }
  else
    std::cerr << "Service call timed out" << std::endl;
}
