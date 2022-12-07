//
// Created by Yuting Xie
// 2022/12/8

#include "DummyScheduler.h"
#include "../utils/utils.h"

namespace gogort {

bool DummyScheduler::DoOnce() {
  // Simply randomly pick the first routine
  for (auto &worker : workers) {
    // Do nothing
  }
  return true;
}

} // namespace gogort