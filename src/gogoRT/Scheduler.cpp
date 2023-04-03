//
// Created by Yuting Xie
// 22-12-8

#include "Scheduler.h"

#include <cassert>

namespace gogort {

Scheduler::Scheduler(std::vector<std::shared_ptr<Worker>> &workers)
    : workers_(workers) {
  num_core = workers.size();
  assert(num_core > 0);
}

bool Scheduler::DoSchedule() { return DoOnce(); }

} // namespace gogort