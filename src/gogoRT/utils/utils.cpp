//
// Created by Yuting Xie
// 2022/12/7

#include "utils.h"
#include <random>

namespace gogort {

std::atomic<uint16> IdGenerator::uuid_ = 0;
uint16 IdGenerator::operator()() {
  // uuid should never be 0
  uuid_ += 1;
  return uuid_.load();
}

} // namespace gogort

static std::default_random_engine random_engine;
int random_int() { return random_engine(); }