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

#ifndef AEROZEN_CLOCK_HPP_
#define AEROZEN_CLOCK_HPP_

#include <chrono>
#include <memory>
#include <string>

#include <gz/utils/SuppressWarning.hh>

namespace aerozen {

/**
 * @brief Clock interface for time tracking.
 */
class Clock {
 public:
  /**
   * @brief Returns the current clock time.
   *
   * @return Current clock time in nanoseconds.
   */
  virtual std::chrono::nanoseconds Time() const = 0;

  /**
   * @brief Returns whether the clock is ready to use.
   *
   * @return True if the clock is ready, false otherwise.
   */
  virtual bool IsReady() const = 0;

  virtual ~Clock() = default;
};

/**
 * @brief Clock implementation driven by gz::msgs::Clock messages on the
 * network.
 */
class NetworkClock : public Clock {
 public:
  /**
   * @brief Time base selection for network clock messages.
   */
  enum class TimeBase : int64_t {
    REAL,  ///< Use Clock message `real` time field as time base.
    SIM,   ///< Use Clock message `sim` time field as time base.
    SYS    ///< Use Clock message `sys` time field as time base.
  };

  /**
   * @brief Constructs a network clock for the given topic and time base.
   *
   * @param[in] _topicName Name of the gz::msgs::Clock topic to use.
   * @param[in] _timeBase Time base for this clock; defaults to simulation time.
   */
  explicit NetworkClock(const std::string &_topicName,
                        TimeBase _timeBase = TimeBase::SIM);

  ~NetworkClock() override;

  std::chrono::nanoseconds Time() const override;

  /**
   * @brief Sets and publishes the given clock time.
   *
   * @param[in] _time Clock time to set.
   *
   * @remarks No clock arbitration is performed.
   */
  void SetTime(std::chrono::nanoseconds _time);

  bool IsReady() const override;

 private:
  class Implementation;

  std::unique_ptr<Implementation> dataPtr;
};

/**
 * @brief Clock implementation using the host OS wall-clock APIs.
 */
class WallClock : public Clock {
 public:
  /**
   * @brief Returns the singleton wall clock instance.
   *
   * @return Pointer to the sole WallClock instance.
   */
  static WallClock *Instance();

  std::chrono::nanoseconds Time() const override;

  bool IsReady() const override;

 private:
  WallClock();

  ~WallClock() override;

  class Implementation;

  std::unique_ptr<Implementation> dataPtr;
};

}  // namespace aerozen

#endif  // AEROZEN_CLOCK_HPP_
