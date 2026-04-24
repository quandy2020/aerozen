/*
 * Copyright (C) 2014 Open Source Robotics Foundation
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

#ifndef AEROZEN_SUBSCRIPTION_HANDLER_HPP_
#define AEROZEN_SUBSCRIPTION_HANDLER_HPP_

#include <google/protobuf/message.h>
#include <google/protobuf/stubs/common.h>
#if GOOGLE_PROTOBUF_VERSION >= 3000000 && GOOGLE_PROTOBUF_VERSION < 4022000
#include <google/protobuf/stubs/casts.h>
#endif

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include <gz/msgs/factory.h>

#include "aerozen/message_info.hpp"
#include "aerozen/subscribe_options.hpp"
#include "aerozen/topic_utils.hpp"
#include "aerozen/transport_types.hpp"
#include "aerozen/uuid.hpp"

namespace zenoh
{
  // Forward declaration.
  class Session;
}

namespace aerozen {
  /**
   * @brief Forward declaration.
   */
  class SubscriptionHandlerBasePrivate;

  /**
   * @brief Contains functions and data common to all subscription handlers.
   */
  class SubscriptionHandlerBase {
    /**
     * @brief Constructor.
     * @param[in] _pUuid UUID of the process registering the handler.
     * @param[in] _nUuid UUID of the node registering the handler.
     * @param[in] _opts Subscription options.
     */
    public: explicit SubscriptionHandlerBase(
      const std::string &_pUuid,
      const std::string &_nUuid,
      const SubscribeOptions &_opts = SubscribeOptions());

    /**
     * @brief Destructor.
     */
    public: virtual ~SubscriptionHandlerBase();

    /**
     * @brief Get the type of subscribed messages.
     * @return String representation of the message type.
     */
    virtual std::string TypeName() = 0;

    /**
     * @brief Get the process UUID.
     * @return The string representation of the process UUID.
     */
    public: std::string ProcUuid() const;

    /**
     * @brief Get the node UUID.
     * @return The string representation of the node UUID.
     */
    public: std::string NodeUuid() const;

    /**
     * @brief Get the unique UUID of this handler.
     * @return A string representation of the handler UUID.
     */
    public: std::string HandlerUuid() const;

    /**
     * @brief Return whether local messages are ignored.
     * @return True when local messages are ignored or false otherwise.
     */
    public: bool IgnoreLocalMessages() const;

    
protected: 
    
    /**
     * @brief Check throttling and whether callback should execute.
     * @return True if callback should execute, false otherwise.
     */
    bool UpdateThrottling();

    /**
     * @brief Subscription options.
     */
    protected: SubscribeOptions opts;

    /**
     * @brief Minimum interval between received messages in nanoseconds.
     */
    protected: double periodNs;

    /**
     * @brief Private data.
     */
    protected: std::unique_ptr<SubscriptionHandlerBasePrivate> dataPtr;
  };

  /**
   * @class ISubscriptionHandler subscription_handler.hpp
   * aerozen/subscription_handler.hpp
   * @brief Interface class used to manage generic protobuf messages.
   *
   * This extends SubscriptionHandlerBase by defining virtual functions for
   * deserializing protobuf message data and receiving deserialized messages.
   * Those functions are not needed by the RawSubscriptionHandler class.
   */
  class ISubscriptionHandler
      : public SubscriptionHandlerBase
  {
    /**
     * @brief Constructor.
     * @param[in] _pUuid UUID of the process registering the handler.
     * @param[in] _nUuid UUID of the node registering the handler.
     * @param[in] _opts Subscription options.
     */
    public: explicit ISubscriptionHandler(
      const std::string &_pUuid,
      const std::string &_nUuid,
      const SubscribeOptions &_opts = SubscribeOptions());

    /**
     * @brief Destructor.
     */
    public: virtual ~ISubscriptionHandler() = default;

    /**
     * @brief Execute the local callback registered for this handler.
     * @param[in] _msg Protobuf message received.
     * @param[in] _info Message information (e.g. topic name).
     * @return True when success, false otherwise.
     */
    public: virtual bool RunLocalCallback(
      const ProtoMsg &_msg,
      const MessageInfo &_info) = 0;

    /**
     * @brief Create a specific protobuf message from serialized data.
     * @param[in] _data Serialized data.
     * @param[in] _type Data type.
     * @return Pointer to the specific protobuf message.
     */
    public: virtual const std::shared_ptr<ProtoMsg> CreateMsg(
      const std::string &_data,
      const std::string &_type) const = 0;

#ifdef HAVE_ZENOH
    /**
     * @brief Create a Zenoh subscriber.
     * @param[in] _session Zenoh session.
     * @param[in] _fullyQualifiedTopic Topic information.
     */
    public: void CreateGenericZenohSubscriber(
      std::shared_ptr<zenoh::Session> _session,
      const FullyQualifiedTopic &_fullyQualifiedTopic);
#endif
  };

  /**
   * @class SubscriptionHandler subscription_handler.hpp
   * @brief Creates a subscription handler for a specific protobuf message.
   *
   * T is the protobuf message type used by this handler.
   */
  template <typename T> class SubscriptionHandler
    : public ISubscriptionHandler
  {
    // Documentation inherited.
    public: explicit SubscriptionHandler(const std::string &_pUuid,
      const std::string &_nUuid,
      const SubscribeOptions &_opts = SubscribeOptions())
      : ISubscriptionHandler(_pUuid, _nUuid, _opts)
    {
    }

    // Documentation inherited.
    const std::shared_ptr<ProtoMsg> CreateMsg(
      const std::string &_data,
      const std::string &/*_type*/) const
    {
      // Instantiate a specific protobuf message
      auto msgPtr = std::make_shared<T>();

      // Create the message using some serialized data
      if (!msgPtr->ParseFromString(_data))
      {
        std::cerr << "SubscriptionHandler::CreateMsg() error: ParseFromString"
                  << " failed" << std::endl;
      }

      return msgPtr;
    }

    // Documentation inherited.
    std::string TypeName()
    {
      return std::string(T().GetTypeName());
    }

    /**
     * @brief Set the callback for this handler.
     * @param[in] _cb Callback.
     */
    void SetCallback(const MsgCallback<T> &_cb)
    {
      this->cb = _cb;
    }

#ifdef HAVE_ZENOH
    /**
     * @brief Set callback and create Zenoh subscriber for this handler.
     * @param[in] _cb Callback.
     * @param[in] _session Zenoh session.
     * @param[in] _fullyQualifiedTopic Topic associated with this callback.
     */
    public: void SetCallback(const MsgCallback<T> &_cb,
                             std::shared_ptr<zenoh::Session> _session,
                             const FullyQualifiedTopic &_fullyQualifiedTopic)
    {
      this->SetCallback(std::move(_cb));
      this->CreateGenericZenohSubscriber(_session, _fullyQualifiedTopic);
    }
#endif

    // Documentation inherited.
    public: bool RunLocalCallback(const ProtoMsg &_msg,
                                  const MessageInfo &_info)
    {
      // No callback stored.
      if (!this->cb)
      {
        std::cerr << "SubscriptionHandler::RunLocalCallback() error: "
                  << "Callback is NULL" << std::endl;
        return false;
      }

      // Check the subscription throttling option.
      if (!this->UpdateThrottling())
        return true;

#if GOOGLE_PROTOBUF_VERSION >= 5028000
      auto msgPtr = google::protobuf::DynamicCastMessage<T>(&_msg);
#elif GOOGLE_PROTOBUF_VERSION >= 4022000
      auto msgPtr = google::protobuf::internal::DownCast<const T*>(&_msg);
#elif GOOGLE_PROTOBUF_VERSION >= 3000000
      auto msgPtr = google::protobuf::down_cast<const T*>(&_msg);
#else
      auto msgPtr = google::protobuf::internal::down_cast<const T*>(&_msg);
#endif

      // Verify the dynamically casted message is valid
      if (msgPtr == nullptr)
      {
        if (_msg.GetDescriptor() != nullptr)
        {
          std::cerr << "SubscriptionHandler::RunLocalCallback() error: "
                    << "Failed to cast the message of the type "
                    << _msg.GetDescriptor()->full_name()
                    << " to the specified type" << '\n';
        }
        else
        {
          std::cerr << "SubscriptionHandler::RunLocalCallback() error: "
                    << "Failed to cast the message of an unknown type"
                    << " to the specified type" << '\n';
        }
        std::cerr.flush();
        return false;
      }

      this->cb(*msgPtr, _info);
      return true;
    }

    /**
     * @brief Callback registered for this handler.
     */
    private: MsgCallback<T> cb;
  };

  /**
   * @brief Specialized template for generic google::protobuf::Message callback.
   */
  template <> class SubscriptionHandler<ProtoMsg>
    : public ISubscriptionHandler
  {
    // Documentation inherited.
    public: explicit SubscriptionHandler(const std::string &_pUuid,
      const std::string &_nUuid,
      const SubscribeOptions &_opts = SubscribeOptions())
      : ISubscriptionHandler(_pUuid, _nUuid, _opts)
    {
    }

    // Documentation inherited.
    public: const std::shared_ptr<ProtoMsg> CreateMsg(
      const std::string &_data,
      const std::string &_type) const
    {
      std::shared_ptr<google::protobuf::Message> msgPtr;

      const google::protobuf::Descriptor *desc =
        google::protobuf::DescriptorPool::generated_pool()
          ->FindMessageTypeByName(_type);

      // First, check if we have the descriptor from the generated proto
      // classes.
      if (desc)
      {
        msgPtr.reset(google::protobuf::MessageFactory::generated_factory()
          ->GetPrototype(desc)->New());
      }
      else
      {
        // Fallback on Gazebo Msgs if the message type is not found.
        msgPtr = gz::msgs::Factory::New(_type);
      }

      if (!msgPtr)
        return nullptr;

      // Create the message using some serialized data
      if (!msgPtr->ParseFromString(_data))
      {
        std::cerr << "CreateMsg() error: ParseFromString failed" << std::endl;
        return nullptr;
      }

      return msgPtr;
    }

    // Documentation inherited.
    public: std::string TypeName()
    {
      return kGenericMessageType;
    }

    /**
     * @brief Set the callback for this handler.
     * @param[in] _cb Callback.
     */
    public: void SetCallback(const MsgCallback<ProtoMsg> &_cb)
    {
      this->cb = _cb;
    }

    /**
     * @brief Set callback and create Zenoh subscriber for this handler.
     * @param[in] _cb Callback.
     * @param[in] _session Zenoh session.
     * @param[in] _fullyQualifiedTopic Topic associated with this callback.
     */
    public: void SetCallback(const MsgCallback<ProtoMsg> &_cb,
                             std::shared_ptr<zenoh::Session> _session,
                             const FullyQualifiedTopic &_fullyQualifiedTopic)
    {
      this->SetCallback(std::move(_cb));
      this->CreateGenericZenohSubscriber(_session, _fullyQualifiedTopic);
    }

    // Documentation inherited.
    public: bool RunLocalCallback(const ProtoMsg &_msg,
                                  const MessageInfo &_info)
    {
      // No callback stored.
      if (!this->cb)
      {
        std::cerr << "SubscriptionHandler::RunLocalCallback() "
                  << "error: Callback is NULL" << std::endl;
        return false;
      }

      // Check the subscription throttling option.
      if (!this->UpdateThrottling())
        return true;

      this->cb(_msg, _info);
      return true;
    }

    /**
     * @brief Callback registered for this handler.
     */
    private: MsgCallback<ProtoMsg> cb;
  };

  /**
   * @brief Manage callback for a raw subscription.
   */
  class RawSubscriptionHandler : public SubscriptionHandlerBase
  {
    /**
     * @brief Constructor.
     * @param[in] _pUuid UUID of the process registering the handler.
     * @param[in] _nUuid UUID of the node registering the handler.
     * @param[in] _msgType Message type name this handler listens for.
     * @param[in] _opts Subscription options.
     */
    public: explicit RawSubscriptionHandler(
      const std::string &_pUuid,
      const std::string &_nUuid,
      const std::string &_msgType = kGenericMessageType,
      const SubscribeOptions &_opts = SubscribeOptions());

    // Documentation inherited
    public: std::string TypeName() override;

    /**
     * @brief Set the callback of this handler.
     * @param[in] _callback Callback triggered when a message is received.
     */
    public: void SetCallback(const RawCallback &_callback);

    /**
     * @brief Set callback and create Zenoh subscriber for this handler.
     * @param[in] _cb Callback.
     * @param[in] _session Zenoh session.
     * @param[in] _fullyQualifiedTopic Topic associated with this callback.
     */
    public: void SetCallback(const RawCallback &_cb,
                             std::shared_ptr<zenoh::Session> _session,
                             const FullyQualifiedTopic &_fullyQualifiedTopic);


    /**
     * @brief Execute the raw callback registered for this handler.
     * @param[in] _msgData Serialized string of message data.
     * @param[in] _size Number of bytes in serialized message data.
     * @param[in] _info Metadata for the message.
     * @return True if callback was triggered, false if callback was not set.
     */
    public: bool RunRawCallback(const char *_msgData, const size_t _size,
                                const MessageInfo &_info);

    /**
     * @brief Destructor.
     */
    public: ~RawSubscriptionHandler();

    private: class Implementation;

    /**
     * @internal
     * @brief Pointer to the implementation of the class.
     */
    private: std::unique_ptr<Implementation> pimpl;
  };
}  // namespace aerozen

#endif  // AEROZEN_SUBSCRIPTION_HANDLER_HPP_
