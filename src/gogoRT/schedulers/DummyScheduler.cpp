//
// Created by Yuting Xie
// 2022/12/8

#include "DummyScheduler.h"
#include "../Worker.h"

namespace gogort {

DummyScheduler::DummyScheduler(std::vector<std::shared_ptr<Worker>> &workers)
    : Scheduler(workers) {}

bool DummyScheduler::DoOnce() {
  // Simply pick the first routine for next worker.
  for (auto worker : workers_) {
    if (worker->isBusy()) {
      continue;
    }
    auto routine = routines_.front();
    routines_.pop_front();
    worker->Assign(routine);
  }
  return true;
}

} // namespace gogort