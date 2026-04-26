/*
 * Copyright (C) 2019 Open Source Robotics Foundation
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

#include "aerozen/proto/stringmsg.pb.h"
#include "gtest/gtest.h"

#include <cstring>
#include <string>

#include "aerozen/ciface.hpp"
#include "aerozen/test/test_utils.hpp"
#include "aerozen/utils/environment.hpp"

static int count;
static char receivedData[256];
static size_t receivedSize;

//////////////////////////////////////////////////
/// \brief Function called each time a topic update is received.
void cb(const char* _data, size_t _size, const char* _msgType,
        void* _userData) {
    int* userData = static_cast<int*>(_userData);

    ASSERT_NE(nullptr, userData);
    EXPECT_EQ(42, *userData);

    aerozen::msgs::StringMsg msg;
    ASSERT_TRUE(msg.ParseFromArray(_data, _size));
    EXPECT_TRUE(std::string(_msgType) == "aerozen.msgs.StringMsg" ||
                std::string(_msgType) == "gz.msgs.StringMsg");
    EXPECT_EQ(msg.data(), "HELLO");
    ++count;
}

//////////////////////////////////////////////////
/// \brief Callback for binary safety test - stores raw data.
void cbBinary(const char* _data, size_t _size, const char*, void*) {
    std::memcpy(receivedData, _data, _size);
    receivedSize = _size;
    ++count;
}

//////////////////////////////////////////////////
/// \brief Function called each time a topic update is received.
void cbNonConst(char* _data, size_t _size, char* _msgType, void* _userData) {
    int* userData = static_cast<int*>(_userData);

    ASSERT_NE(nullptr, userData);
    EXPECT_EQ(42, *userData);

    aerozen::msgs::StringMsg msg;
    ASSERT_TRUE(msg.ParseFromArray(_data, _size));
    EXPECT_TRUE(std::string(_msgType) == "aerozen.msgs.StringMsg" ||
                std::string(_msgType) == "gz.msgs.StringMsg");
    EXPECT_EQ(msg.data(), "HELLO");
    ++count;
}

//////////////////////////////////////////////////
TEST(CIfaceTest, PubSub) {
    count = 0;
    TransportNode* node = TransportNodeCreate(nullptr);
    EXPECT_NE(nullptr, node);

    const char* topic = "/foo";

    int userData = 42;

    // Subscribe
    ASSERT_EQ(0, TransportSubscribe(node, topic, cb, &userData));

    // Subscribe
    ASSERT_EQ(0, TransportSubscribeNonConst(node, const_cast<char*>(topic),
                                            cbNonConst, &userData));

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
    void* buffer = malloc(size);

    ASSERT_NE(nullptr, buffer);

    // Serialize the message.
    ASSERT_TRUE(msg.SerializeToArray(buffer, size));

    EXPECT_EQ(0, TransportPublishRaw(node, topic, buffer, size,
                                     msg.GetTypeName().data()));

    EXPECT_EQ(2, count);

    count = 0;

    // Unsubscribe
    ASSERT_EQ(0, TransportUnsubscribe(node, topic));
    EXPECT_EQ(0, TransportPublishRaw(node, topic, buffer, size,
                                     msg.GetTypeName().data()));
    EXPECT_EQ(0, count);

    free(buffer);
    TransportNodeDestroy(&node);
    EXPECT_EQ(nullptr, node);
}

//////////////////////////////////////////////////
TEST(CIfaceTest, PubSubPartitions) {
    count = 0;
    TransportNode* node = TransportNodeCreate(nullptr);
    TransportNode* nodeBar = TransportNodeCreate("bar");
    EXPECT_NE(nullptr, node);

    const char* topic = "/foo";

    int userData = 42;

    // Subscribe on "bar" topic
    ASSERT_EQ(0, TransportSubscribe(nodeBar, topic, cb, &userData));

    // Subscribe
    ASSERT_EQ(0, TransportSubscribeNonConst(node, const_cast<char*>(topic),
                                            cbNonConst, &userData));

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
    void* buffer = malloc(size);

    ASSERT_NE(nullptr, buffer);

    // Serialize the message.
    ASSERT_TRUE(msg.SerializeToArray(buffer, size));

    // Publish on "bar" partition
    EXPECT_EQ(0, TransportPublishRaw(nodeBar, topic, buffer, size,
                                     msg.GetTypeName().data()));
    EXPECT_EQ(1, count);

    // Publish on default partition
    EXPECT_EQ(0, TransportPublishRaw(nodeBar, topic, buffer, size,
                                     msg.GetTypeName().data()));
    EXPECT_EQ(2, count);

    count = 0;

    // Unsubscribe
    ASSERT_EQ(0, TransportUnsubscribe(nodeBar, topic));
    EXPECT_EQ(0, TransportPublishRaw(nodeBar, topic, buffer, size,
                                     msg.GetTypeName().data()));
    EXPECT_EQ(0, count);

    free(buffer);
    TransportNodeDestroy(&node);
    EXPECT_EQ(nullptr, node);
    TransportNodeDestroy(&nodeBar);
    EXPECT_EQ(nullptr, nodeBar);
}

//////////////////////////////////////////////////
TEST(CIfaceTest, PublishRawBinarySafe) {
    count = 0;
    std::memset(receivedData, 0, sizeof(receivedData));
    receivedSize = 0;

    TransportNode* node = TransportNodeCreate(nullptr);
    EXPECT_NE(nullptr, node);

    const char* topic = "/binary_test";
    const char* msgType = "gz.msgs.Bytes";

    // Subscribe to receive raw binary data
    ASSERT_EQ(0, TransportSubscribe(node, topic, cbBinary, nullptr));

    // Test data with embedded null byte - "Hel\0lo" (6 bytes)
    const char binaryData[] = {'H', 'e', 'l', '\0', 'l', 'o'};
    size_t dataSize = sizeof(binaryData);  // 6 bytes

    // Advertise the topic first
    ASSERT_EQ(0, TransportAdvertise(node, topic, msgType));

    // Publish using the new binary-safe function
    EXPECT_EQ(
        0, TransportPublishRaw(node, topic, binaryData, dataSize, msgType));

    // Verify subscriber receives all 6 bytes
    EXPECT_EQ(1, count);
    EXPECT_EQ(dataSize, receivedSize);
    EXPECT_EQ(0, std::memcmp(binaryData, receivedData, dataSize));

    // Verify the data after the null byte is preserved
    EXPECT_EQ('l', receivedData[4]);
    EXPECT_EQ('o', receivedData[5]);

    TransportNodeDestroy(&node);
    EXPECT_EQ(nullptr, node);
}

//////////////////////////////////////////////////
int main(int argc, char** argv) {
    // Get a random partition name.
    std::string partition = testing::GetRandomNumber();

    // Set the partition name for this process.
    aerozen::utils::setenv("AEROZEN_PARTITION", partition);

    // Enable verbose mode.
    // gz::utils::setenv("GZ_VERBOSE", "1");

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
