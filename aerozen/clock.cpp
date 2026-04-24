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

#include "aerozen/clock.hpp"

#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <mutex>

#include <gz/msgs/clock.pb.h>
#include <gz/msgs/time.pb.h>
#include "aerozen/node.hpp"

namespace aerozen {

class NetworkClock::Implementation {
 public:
  /**
   * @brief Constructs the pimpl for a network clock.
   *
   * @param[in] _topicName Name of the gz::msgs::Clock topic to use.
   * @param[in] _timeBase Time base for this clock.
   */
  Implementation(const std::string &_topicName,
                 NetworkClock::TimeBase _time_base);

  /**
   * @brief Returns the current clock time (thread-safe read).
   *
   * @return Current clock time in nanoseconds.
   */
  std::chrono::nanoseconds Time() const;

  /**
   * @brief Sets and publishes the given clock time.
   *
   * @param[in] _time Clock time to set.
   *
   * @remarks No clock arbitration is performed.
   */
  void SetTime(std::chrono::nanoseconds _time);

  /**
   * @brief Updates stored time from a time message (thread-safe write).
   *
   * @param[in] _msg Time fields from a clock message.
   */
  void UpdateTimeFromMessage(const gz::msgs::Time &_msg);

  /**
   * @brief Subscriber callback for incoming clock messages.
   *
   * @param[in] _msg Received clock message.
   */
  void OnClockMessageReceived(const gz::msgs::Clock &_msg);

  std::chrono::nanoseconds clock_time_ns;
  NetworkClock::TimeBase clock_time_base;
  mutable std::mutex clock_mutex;
  Node node;
  Node::Publisher pub;
};

NetworkClock::Implementation::Implementation(const std::string &_topicName,
                                             NetworkClock::TimeBase _time_base)
    : clock_time_ns(std::chrono::nanoseconds::zero()),
      clock_time_base(_time_base) {
  if (!node.Subscribe(_topicName, &Implementation::OnClockMessageReceived,
                       this)) {
    std::cerr << "Could not subscribe to [" << _topicName << "] topic\n";
  }
  pub = node.Advertise<gz::msgs::Clock>(_topicName);
}

std::chrono::nanoseconds NetworkClock::Implementation::Time() const {
  std::lock_guard<std::mutex> lock(clock_mutex);
  return clock_time_ns;
}

void NetworkClock::Implementation::SetTime(std::chrono::nanoseconds _time) {
  const std::chrono::seconds time_as_secs =
      std::chrono::duration_cast<std::chrono::seconds>(_time);
  const int secs = static_cast<int>(time_as_secs.count());
  const int nsecs = static_cast<int>(
      (_time - std::chrono::duration_cast<std::chrono::nanoseconds>(
                   time_as_secs))
          .count());

  gz::msgs::Clock msg;
  switch (clock_time_base) {
    case NetworkClock::TimeBase::SIM:
      msg.mutable_sim()->set_sec(secs);
      msg.mutable_sim()->set_nsec(nsecs);
      break;
    case NetworkClock::TimeBase::REAL:
      msg.mutable_real()->set_sec(secs);
      msg.mutable_real()->set_nsec(nsecs);
      break;
    case NetworkClock::TimeBase::SYS:
      msg.mutable_system()->set_sec(secs);
      msg.mutable_system()->set_nsec(nsecs);
      break;
    default:
      std::cerr << "Invalid clock time base\n";
      return;
  }
  // Distributes clock message to every subscriber, including this object.
  pub.Publish(msg);
}

void NetworkClock::Implementation::UpdateTimeFromMessage(
    const gz::msgs::Time &_msg) {
  std::lock_guard<std::mutex> lock(clock_mutex);
  clock_time_ns = std::chrono::seconds(_msg.sec()) +
                  std::chrono::nanoseconds(_msg.nsec());
}

void NetworkClock::Implementation::OnClockMessageReceived(
    const gz::msgs::Clock &_msg) {
  switch (clock_time_base) {
    case NetworkClock::TimeBase::REAL:
      if (_msg.has_real()) {
        UpdateTimeFromMessage(_msg.real());
      } else {
        std::cerr << "Real time not present in clock message\n";
      }
      break;
    case NetworkClock::TimeBase::SIM:
      if (_msg.has_sim()) {
        UpdateTimeFromMessage(_msg.sim());
      } else {
        std::cerr << "Sim time not present in clock message\n";
      }
      break;
    case NetworkClock::TimeBase::SYS:
      if (_msg.has_system()) {
        UpdateTimeFromMessage(_msg.system());
      } else {
        std::cerr << "System time not present in clock message\n";
      }
      break;
    default:
      std::cerr << "Invalid clock time base\n";
      break;
  }
}

NetworkClock::NetworkClock(const std::string &_topicName, TimeBase _time_base)
    : dataPtr(std::make_unique<Implementation>(_topicName, _time_base)) {}

NetworkClock::~NetworkClock() = default;

std::chrono::nanoseconds NetworkClock::Time() const {
  return dataPtr->Time();
}

void NetworkClock::SetTime(std::chrono::nanoseconds _time) {
  dataPtr->SetTime(_time);
}

bool NetworkClock::IsReady() const {
  // At least one non-zero clock message has arrived.
  return dataPtr->Time().count() != 0;
}

class WallClock::Implementation {
 public:
  Implementation();

  /**
   * @brief Returns wall clock time in nanoseconds (UTC-based).
   */
  std::chrono::nanoseconds Time() const;

  /** Offset so steady clock maps to UTC, in nanoseconds. */
  std::chrono::nanoseconds wall_minus_mono;
};

WallClock::Implementation::Implementation() {
  const std::chrono::nanoseconds wall_start_ns(
      std::chrono::seconds(std::time(nullptr)));
  const std::chrono::nanoseconds mono_start_ns(
      std::chrono::steady_clock::now().time_since_epoch());
  wall_minus_mono = wall_start_ns - mono_start_ns;
}

std::chrono::nanoseconds WallClock::Implementation::Time() const {
  const std::chrono::nanoseconds now_ns(
      std::chrono::steady_clock::now().time_since_epoch());
  return wall_minus_mono + now_ns;
}

WallClock *WallClock::Instance() {
  static WallClock clock;
  return &clock;
}

WallClock::WallClock() : dataPtr(std::make_unique<Implementation>()) {}

WallClock::~WallClock() = default;

std::chrono::nanoseconds WallClock::Time() const {
  return dataPtr->Time();
}

bool WallClock::IsReady() const {
  return true;
}

}  // namespace aerozen
