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

#include <signal.h>
#include <unistd.h>

#include "aerozen/msgs/stringmsg.pb.h"
#include "aerozen/ciface.hpp"

static bool g_terminatePub = false;

/**
 * @brief Function callback executed when a SIGINT or SIGTERM signals are
 * captured. This is used to break the infinite loop that publishes messages
 * and exit the program smoothly.
 */
void signalHandler(int _signal)
{
  if (_signal == SIGINT || _signal == SIGTERM)
    g_terminatePub = true;
}

int main(int argc, char **argv)
{
  // Install a signal handler for SIGINT and SIGTERM.
  signal(SIGINT,  signalHandler);
  signal(SIGTERM, signalHandler);

  // Create a transport node.
  aerozen::TransportNode *node = aerozen::TransportNodeCreate(nullptr);
  aerozen::TransportNode *nodeRed = aerozen::TransportNodeCreate("red");

  const char *topic = "/foo";

  // Prepare the message.
  aerozen::msgs::StringMsg msg;
  msg.set_data("HELLO");

  // Get the size of the serialized message
#if GOOGLE_PROTOBUF_VERSION >= 3004000
  int size = msg.ByteSizeLong();
#else
  int size = msg.ByteSize();
#endif

  // Allocate space for the serialized message
  void *buffer = malloc(size);

  // Serialize the message.
  msg.SerializeToArray(buffer, size);

  // Prepare the message.
  aerozen::msgs::StringMsg msgRed;
  msgRed.set_data("RED HELLO");

  // Get the size of the serialized message
#if GOOGLE_PROTOBUF_VERSION >= 3004000
  int sizeRed = msgRed.ByteSizeLong();
#else
  int sizeRed = msgRed.ByteSize();
#endif

  // Allocate space for the serialized message
  void *bufferRed = malloc(sizeRed);

  // Serialize the message.
  msgRed.SerializeToArray(bufferRed, sizeRed);

  // Publish messages at 1Hz.
  while (!g_terminatePub) {
    aerozen::TransportPublishRaw(node, topic, buffer, size, msg.GetTypeName().c_str());
    aerozen::TransportPublishRaw(nodeRed, topic, bufferRed, sizeRed, msgRed.GetTypeName().c_str());

    printf("Publishing hello on topic %s.\n", topic);
    sleep(1);
  }

  free(buffer);
  free(bufferRed);
  aerozen::TransportNodeDestroy(&node);
  aerozen::TransportNodeDestroy(&nodeRed);

  return 0;
}
