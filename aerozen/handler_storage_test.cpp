/*
 * Basic tests for HandlerStorage.
 */

#include "gtest/gtest.h"

#include "aerozen/handler_storage.hpp"
#include "aerozen/proto/int32.pb.h"
#include "aerozen/proto/vector3d.pb.h"
#include "aerozen/rep_handler.hpp"
#include "aerozen/subscription_handler.hpp"

namespace {
bool RepCb(const aerozen::msgs::Vector3d&, aerozen::msgs::Int32& _rep) {
  _rep.set_data(7);
  return true;
}
}  // namespace

TEST(HandlerStorageTest, RepHandlerStorageBasic) {
  aerozen::HandlerStorage<aerozen::IRepHandler> storage;
  auto rep = std::make_shared<
      aerozen::RepHandler<aerozen::msgs::Vector3d, aerozen::msgs::Int32>>(
      "proc", "node");
  rep->SetCallback(RepCb);

  storage.AddHandler("/topic", "node", rep);
  EXPECT_TRUE(storage.HasHandlersForTopic("/topic"));
  EXPECT_TRUE(storage.HasHandlersForNode("/topic", "node"));

  std::shared_ptr<aerozen::IRepHandler> out;
  EXPECT_TRUE(storage.FirstHandler(
      "/topic", aerozen::msgs::Vector3d().GetTypeName(),
      aerozen::msgs::Int32().GetTypeName(), out));
  ASSERT_TRUE(out != nullptr);

  aerozen::msgs::Vector3d req;
  aerozen::msgs::Int32 res;
  EXPECT_TRUE(out->RunLocalCallback(req, res));
  EXPECT_EQ(7, res.data());
}

TEST(HandlerStorageTest, SubscriptionHandlerStorageBasic) {
  aerozen::HandlerStorage<aerozen::ISubscriptionHandler> storage;
  auto sub = std::make_shared<aerozen::SubscriptionHandler<aerozen::msgs::Int32>>(
      "proc", "node");

  storage.AddHandler("/topic", "node", sub);
  EXPECT_TRUE(storage.HasHandlersForTopic("/topic"));

  std::shared_ptr<aerozen::ISubscriptionHandler> out;
  EXPECT_TRUE(storage.FirstHandler(
      "/topic", std::string(aerozen::msgs::Int32().GetTypeName()), out));
  ASSERT_TRUE(out != nullptr);
  EXPECT_EQ("node", out->NodeUuid());
}
