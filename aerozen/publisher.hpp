/*
 * Copyright (C) 2015 Open Source Robotics Foundation
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

#ifndef AEROZEN_PUBLISHER_HPP_
#define AEROZEN_PUBLISHER_HPP_

#include <gz/msgs/discovery.pb.h>

#include <iostream>
#include <string>

#include "aerozen/advertise_options.hpp"

namespace aerozen {

/// \class Publisher publisher.hpp
/// aerozen/publisher.hpp
/// \brief This class stores all the information about a publisher.
/// It stores the topic name that publishes, addresses, UUIDs, scope, etc.
class Publisher
{
public:
    /// \brief Default constructor.
    Publisher() = default;

    /// \brief Constructor.
    /// \param[in] _topic Topic name.
    /// \param[in] _addr ZeroMQ address.
    /// \param[in] _pUuid Process UUID.
    /// \param[in] _nUuid node UUID.
    /// \param[in] _opts The advertise options.
    Publisher(const std::string& _topic, const std::string& _addr,
              const std::string& _pUuid, const std::string& _nUuid,
              const AdvertiseOptions& _opts);

    /// \brief Destructor.
    virtual ~Publisher() = default;

    /// \brief Get the topic published by this publisher.
    /// \return Topic name.
    /// \sa SetTopic.
    std::string Topic() const;

    /// \brief Get the ZeroMQ address of the publisher.
    /// \return ZeroMQ address.
    /// \sa SetAddr.
    std::string Addr() const;

    /// \brief Get the process UUID of the publisher.
    /// return Process UUID.
    /// \sa SetPUuid.
    std::string PUuid() const;

    /// \brief Get the node UUID of the publisher.
    /// \return Node UUID.
    /// \sa SetNUuid.
    std::string NUuid() const;

    /// \brief Get the advertised options.
    /// \return The advertised options.
    /// \sa SetOptions.
    virtual const AdvertiseOptions& Options() const;

    /// \brief Set the topic name published by this publisher.
    /// \param[in] _topic New topic name.
    /// \sa Topic.
    void SetTopic(const std::string& _topic);

    /// \brief Set ZeroMQ address of the publisher.
    /// \param[in] _addr New address.
    /// \sa Addr.
    void SetAddr(const std::string& _addr);

    /// \brief Set the process UUID of the publisher.
    /// \param[in] _pUuid New process UUID.
    /// \sa PUuid.
    void SetPUuid(const std::string& _pUuid);

    /// \brief Set the node UUID of the publisher.
    /// \param[in] _nUuid New node UUID.
    /// \sa NUuid.
    void SetNUuid(const std::string& _nUuid);

    /// \brief Set the advertised options.
    /// \param[in] _opts New advertised options.
    /// \sa Options.
    void SetOptions(const AdvertiseOptions& _opts);

    /// \brief Populate a discovery message.
    /// \param[in] _msg Message to fill.
    virtual void FillDiscovery(msgs::Discovery& _msg) const;

    /// \brief Set data from a discovery message.
    /// \param[in] _msg Discovery message.
    virtual void SetFromDiscovery(const msgs::Discovery& _msg);

    /// \brief Equality operator. This function checks if the given
    /// publisher has identical Topic, Addr, PUuid, NUuid, and Scope
    /// strings to this object.
    /// \param[in] _pub The publisher to compare against.
    /// \return True if this object matches the provided object.
    bool operator==(const Publisher& _pub) const;

    /// \brief Inequality operator. This function checks if the given
    /// publisher does not have identical Topic, Addr, PUuid, NUuid, and Scope
    /// strings to this object.
    /// \param[in] _pub The publisher to compare against.
    /// \return True if this object does not match the provided object.
    bool operator!=(const Publisher& _pub) const;

    /// \brief Stream insertion operator.
    /// \param[out] _out The output stream.
    /// \param[in] _msg Publisher to write to the stream.
    friend std::ostream& operator<<(std::ostream& _out, const Publisher& _msg) {
        _out << "Publisher:" << std::endl
             << "\tTopic: [" << _msg.Topic() << "]" << std::endl
             << "\tAddress: " << _msg.Addr() << std::endl
             << "\tProcess UUID: " << _msg.PUuid() << std::endl
             << "\tNode UUID: " << _msg.NUuid() << std::endl
             << _msg.Options();

        return _out;
    }

protected:
    /// \brief Topic name.
    std::string topic;

    /// \brief ZeroMQ address of the publisher.
    std::string addr;

    /// \brief Process UUID of the publisher.
    std::string pUuid;

    /// \brief Node UUID of the publisher.
    std::string nUuid;

private:
    /// \brief Advertised options.
    /// This member is not used when we have a derived publisher.
    AdvertiseOptions opts;
};

/// \class MessagePublisher publisher.hpp
/// aerozen/publisher.hpp
/// \brief This class stores all the information about a message publisher.
class MessagePublisher : public Publisher
{
public:
    /// \brief Default constructor.
    MessagePublisher() = default;

    /// \brief Constructor.
    /// \param[in] _topic Topic name.
    /// \param[in] _addr ZeroMQ address.
    /// \param[in] _ctrl ZeroMQ control address.
    /// \param[in] _pUuid Process UUID.
    /// \param[in] _nUuid node UUID.
    /// \param[in] _msgTypeName Message type advertised by this publisher.
    /// \param[in] _opts Advertise options.
    explicit MessagePublisher(const std::string& _topic,
                              const std::string& _addr,
                              const std::string& _ctrl,
                              const std::string& _pUuid,
                              const std::string& _nUuid,
                              const std::string& _msgTypeName,
                              const AdvertiseMessageOptions& _opts);

    /// \brief Destructor.
    virtual ~MessagePublisher() = default;

    /// \brief Get the ZeroMQ control address. This address is used by the
    /// subscribers to notify the publisher about the new subscription.
    /// \return ZeroMQ control address of the publisher.
    /// \sa SetCtrl.
    std::string Ctrl() const;

    /// \brief Set the ZeroMQ control address of the publisher.
    /// \param[in] _ctrl New control address.
    /// \sa Ctrl.
    void SetCtrl(const std::string& _ctrl);

    /// \brief Get the message type advertised by this publisher.
    /// \return Message type.
    std::string MsgTypeName() const;

    /// \brief Set the message type advertised by this publisher.
    /// \param[in] _msgTypeName New message type.
    /// \sa MsgTypeName.
    void SetMsgTypeName(const std::string& _msgTypeName);

    /// \brief Get the advertised options.
    /// \return The advertised options.
    /// \sa SetOptions.
    virtual const AdvertiseMessageOptions& Options() const;

    /// \brief Set the advertised options.
    /// \param[in] _opts New advertised options.
    /// \sa Options.
    void SetOptions(const AdvertiseMessageOptions& _opts);

    /// \brief Populate a discovery message.
    /// \param[in] _msg Message to fill.
    virtual void FillDiscovery(msgs::Discovery& _msg) const final;

    /// \brief Set data from a discovery message.
    /// \param[in] _msg Discovery message.
    virtual void SetFromDiscovery(const msgs::Discovery& _msg);

    /// \brief Stream insertion operator.
    /// \param[out] _out The output stream.
    /// \param[in] _msg MessagePublisher to write to the stream.
    friend std::ostream& operator<<(std::ostream& _out,
                                    const MessagePublisher& _msg) {
        _out << "Publisher:" << std::endl
             << "\tTopic: [" << _msg.Topic() << "]" << std::endl
             << "\tAddress: " << _msg.Addr() << std::endl
             << "\tProcess UUID: " << _msg.PUuid() << std::endl
             << "\tNode UUID: " << _msg.NUuid() << std::endl
             << "\tControl address: " << _msg.Ctrl() << std::endl
             << "\tMessage type: " << _msg.MsgTypeName() << std::endl
             << _msg.Options();
        return _out;
    }

    /// \brief Equality operator. This function checks if the given
    /// message publisher has identical Topic, Addr, PUuid, NUuid, Scope,
    /// Ctrl, and MsgTypeName strings to this object.
    /// \param[in] _pub The message publisher to compare against.
    /// \return True if this object matches the provided object.
    bool operator==(const MessagePublisher& _pub) const;

    /// \brief Inequality operator. This function checks if the given
    /// message publisher does not have identical Topic, Addr, PUuid, NUuid,
    /// Scope, Ctrl, and MsgTypeName strings to this object.
    /// \param[in] _pub The message publisher to compare against.
    /// \return True if this object does not match the provided object.
    bool operator!=(const MessagePublisher& _pub) const;

private:
    /// \brief ZeroMQ control address of the publisher.
    std::string ctrl;

    /// \brief Message type advertised by this publisher.
    std::string msgTypeName;

    /// \brief Advertise options (e.g.: msgsPerSec).
    AdvertiseMessageOptions msgOpts;
};

/// \class ServicePublisher publisher.hpp
/// aerozen/publisher.hpp
/// \brief This class stores all the information about a service publisher.
class ServicePublisher : public Publisher
{
public:
    /// \brief Default constructor.
    ServicePublisher() = default;

    /// \brief Constructor.
    /// \param[in] _topic Topic name.
    /// \param[in] _addr ZeroMQ address.
    /// \param[in] _id ZeroMQ socket ID.
    /// \param[in] _pUuid Process UUID.
    /// \param[in] _nUuid node UUID.
    /// \param[in] _reqType Message type used in the service request.
    /// \param[in] _repType Message type used in the service response.
    /// \param[in] _opts Advertise options.
    ServicePublisher(const std::string& _topic, const std::string& _addr,
                     const std::string& _id, const std::string& _pUuid,
                     const std::string& _nUuid, const std::string& _reqType,
                     const std::string& _repType,
                     const AdvertiseServiceOptions& _opts);

    /// \brief Destructor.
    virtual ~ServicePublisher() = default;

    /// \brief Get the ZeroMQ socket ID used by this publisher.
    /// \return The socket ID.
    /// \sa SetSocketId.
    std::string SocketId() const;

    /// \brief Set the ZeroMQ socket ID for this publisher.
    /// \param[in] _socketId New socket ID.
    /// \sa SocketId.
    void SetSocketId(const std::string& _socketId);

    /// \brief Get the name of the request's protobuf message advertised.
    /// \return The protobuf message type.
    /// \sa SetReqTypeName.
    std::string ReqTypeName() const;

    /// \brief Get the name of the response's protobuf message advertised.
    /// \return The protobuf message type.
    /// \sa SetRepTypeName.
    std::string RepTypeName() const;

    /// \brief Set the name of the request's protobuf message advertised.
    /// \param[in] _reqTypeName The protobuf message type.
    /// \sa ReqTypeName.
    void SetReqTypeName(const std::string& _reqTypeName);

    /// \brief Set the name of the response's protobuf message advertised.
    /// \param[in] _repTypeName The protobuf message type.
    /// \sa RepTypeName.
    void SetRepTypeName(const std::string& _repTypeName);

    /// \brief Get the advertised options.
    /// \return The advertised options.
    /// \sa SetOptions.
    virtual const AdvertiseServiceOptions& Options() const;

    /// \brief Set the advertised options.
    /// \param[in] _opts New advertised options.
    /// \sa Options.
    void SetOptions(const AdvertiseServiceOptions& _opts);

    /// \brief Populate a discovery message.
    /// \param[in] _msg Message to fill.
    virtual void FillDiscovery(msgs::Discovery& _msg) const final;

    /// \brief Populate a discovery message.
    /// \brief Set data from a discovery message.
    /// \param[in] _msg Discovery message.
    virtual void SetFromDiscovery(const msgs::Discovery& _msg);

    /// \brief Stream insertion operator.
    /// \param[out] _out The output stream.
    /// \param[in] _msg ServicePublisher to write to the stream.
    friend std::ostream& operator<<(std::ostream& _out,
                                    const ServicePublisher& _msg) {
        _out << "Publisher:" << std::endl
             << "\tTopic: [" << _msg.Topic() << "]" << std::endl
             << "\tAddress: " << _msg.Addr() << std::endl
             << "\tProcess UUID: " << _msg.PUuid() << std::endl
             << "\tNode UUID: " << _msg.NUuid() << std::endl
             << "\tSocket ID: " << _msg.SocketId() << std::endl
             << "\tRequest type: " << _msg.ReqTypeName() << std::endl
             << "\tResponse type: " << _msg.RepTypeName() << std::endl
             << _msg.Options();

        return _out;
    }

    /// \brief Equality operator. This function checks if the given
    /// service has identical Topic, Addr, PUuid, NUuid, Scope,
    /// SocketId, ReqTypeName, RepTypeName strings to this object.
    /// \param[in] _srv The service publisher to compare against.
    /// \return True if this object matches the provided object.
    bool operator==(const ServicePublisher& _srv) const;

    /// \brief Inequality operator. This function checks if the given
    /// service does not have identical Topic, Addr, PUuid, NUuid, Scope,
    /// SocketId, ReqTypeName, RepTypeName strings to this object.
    /// \param[in] _srv The service publisher to compare against.
    /// \return True if this object does not match the provided object.
    bool operator!=(const ServicePublisher& _srv) const;

private:
    /// \brief ZeroMQ socket ID used by this publisher.
    std::string socketId;

    /// \brief The name of the request's protobuf message advertised.
    std::string reqTypeName;

    /// \brief The name of the response's protobuf message advertised.
    std::string repTypeName;

    /// \brief Advertise options.
    AdvertiseServiceOptions srvOpts;
};
}  // namespace aerozen

#endif  // AEROZEN_PUBLISHER_HPP_
