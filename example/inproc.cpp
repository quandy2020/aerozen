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

/*
 * In-process pub/sub example with timestamps.
 */

#include <atomic>
#include <chrono>
#include <csignal>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include "aerozen/node.hpp"
#include "aerozen/proto/stringmsg.pb.h"

namespace {
std::atomic<bool> g_terminate{false};
std::mutex g_logMutex;
std::mutex g_statMutex;

struct DelayStats {
  std::uint64_t count{0};
  std::int64_t minMs{std::numeric_limits<std::int64_t>::max()};
  std::int64_t maxMs{0};
  long double sumMs{0.0L};
};

DelayStats g_stats;

std::string NowString() {
  const auto now = std::chrono::system_clock::now();
  const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch()) % 1000;
  const std::time_t t = std::chrono::system_clock::to_time_t(now);

  std::tm tm_buf{};
  localtime_r(&t, &tm_buf);

  std::ostringstream oss;
  oss << std::put_time(&tm_buf, "%F %T")
      << '.' << std::setw(3) << std::setfill('0') << ms.count();
  return oss.str();
}

std::int64_t NowMs() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

void LogLine(const std::string &_line) {
  std::lock_guard<std::mutex> lock(g_logMutex);
  std::cout << _line << std::endl;
}

std::string BuildPayload(int _seq, std::int64_t _sendMs, const std::string &_content) {
  std::ostringstream oss;
  oss << _seq << "|" << _sendMs << "|" << _content;
  return oss.str();
}

bool ParsePayload(const std::string &_payload, int &_seq, std::int64_t &_sendMs,
                  std::string &_content) {
  const auto firstDelimPos = _payload.find('|');
  if (firstDelimPos == std::string::npos) {
    return false;
  }
  const auto secondDelimPos = _payload.find('|', firstDelimPos + 1);
  if (secondDelimPos == std::string::npos) {
    return false;
  }

  try {
    _seq = std::stoi(_payload.substr(0, firstDelimPos));
    _sendMs =
        std::stoll(_payload.substr(firstDelimPos + 1, secondDelimPos - firstDelimPos - 1));
    _content = _payload.substr(secondDelimPos + 1);
  } catch (...) {
    return false;
  }
  return true;
}

void PrintSummary() {
  std::uint64_t count = 0;
  std::int64_t minMs = 0;
  std::int64_t maxMs = 0;
  long double avgMs = 0.0L;
  {
    std::lock_guard<std::mutex> statLock(g_statMutex);
    count = g_stats.count;
    if (count > 0) {
      minMs = g_stats.minMs;
      maxMs = g_stats.maxMs;
      avgMs = g_stats.sumMs / static_cast<long double>(count);
    }
  }

  std::ostringstream oss;
  oss << "[" << NowString() << "] summary: count=" << count;
  if (count > 0) {
    oss << " avg_ms=" << std::fixed << std::setprecision(2) << avgMs
        << " min_ms=" << minMs << " max_ms=" << maxMs;
  }
  LogLine(oss.str());
}

void SignalHandler(int _sig) {
  if (_sig == SIGINT || _sig == SIGTERM) {
    g_terminate = true;
  }
}

void OnMsg(const aerozen::msgs::StringMsg &_msg) {
  int seq = -1;
  std::int64_t sendMs = 0;
  std::string content;
  if (!ParsePayload(_msg.data(), seq, sendMs, content)) {
    LogLine("[" + NowString() + "] rece: invalid payload [" + _msg.data() + "]");
    return;
  }

  const std::int64_t recvMs = NowMs();
  const std::int64_t delayMs = recvMs - sendMs;

  std::uint64_t count = 0;
  std::int64_t minMs = 0;
  std::int64_t maxMs = 0;
  long double avgMs = 0.0L;
  {
    std::lock_guard<std::mutex> statLock(g_statMutex);
    g_stats.count++;
    g_stats.sumMs += static_cast<long double>(delayMs);
    if (delayMs < g_stats.minMs) {
      g_stats.minMs = delayMs;
    }
    if (delayMs > g_stats.maxMs) {
      g_stats.maxMs = delayMs;
    }

    count = g_stats.count;
    minMs = g_stats.minMs;
    maxMs = g_stats.maxMs;
    avgMs = g_stats.sumMs / static_cast<long double>(count);
  }

  std::ostringstream oss;
  oss << "[" << NowString() << "] rece: seq=" << seq << " content=\"" << content
      << "\" send_ms=" << sendMs << " recv_ms=" << recvMs << " delay_ms=" << delayMs
      << " avg_ms=" << std::fixed << std::setprecision(2) << avgMs
      << " min_ms=" << minMs << " max_ms=" << maxMs;
  LogLine(oss.str());
}
}  // namespace

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  const std::string topic = "/inproc";

  // Use separate nodes in the same process for in-process transport path.
  aerozen::Node subscriberNode;
  aerozen::Node publisherNode;

  if (!subscriberNode.Subscribe(topic, OnMsg)) {
    std::cerr << "Failed to subscribe [" << topic << "]" << std::endl;
    return -1;
  }

  auto pub = publisherNode.Advertise<aerozen::msgs::StringMsg>(topic);
  if (!pub) {
    std::cerr << "Failed to advertise [" << topic << "]" << std::endl;
    return -1;
  }

  int seq = 0;
  while (!g_terminate) {
    const std::string content = "inproc message #" + std::to_string(seq);
    const std::int64_t sendMs = NowMs();
    aerozen::msgs::StringMsg msg;
    msg.set_data(BuildPayload(seq, sendMs, content));
    if (!pub.Publish(msg)) {
      std::cerr << "Publish failed" << std::endl;
      break;
    }

    {
      std::ostringstream oss;
      oss << "[" << NowString() << "] send: seq=" << seq << " content=\"" << content
          << "\" send_ms=" << sendMs;
      LogLine(oss.str());
    }
    ++seq;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  PrintSummary();
  return 0;
}
