//
// Created by Yuting Xie
// 2022/12/7

#include "utils.h"
#include <random>

uuid_t get_next_uuid() {
  static std::atomic<uint16> uuid_ = 0;
  uuid_ += 1;
  return uuid_.load();
}

static std::default_random_engine random_engine;
int random_int() { return random_engine(); }