//
// Created by Yuting Xie
// 2022/12/7

#include "utils.h"

#include <chrono>
#include <random>

gogo_id_t get_next_uuid() {
  static std::atomic<uint16> uuid_ = 0;
  uuid_ += 1;
  return uuid_.load();
}

static std::default_random_engine random_engine;
int random_int() { return random_engine(); }

uint64 get_current_timestamp_ms() {
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(duration)
      .count();
}