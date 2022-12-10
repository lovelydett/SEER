//
// Created by Yuting Xie
// 2022/12/8

#include "DummyScheduler.h"
#include "../Worker.h"
#include <glog/logging.h>

namespace gogort {

DummyScheduler::DummyScheduler(std::vector<std::shared_ptr<Worker>> &workers)
    : Scheduler(workers) {}

bool DummyScheduler::DoOnce() {
  // Simply pick the first routine for next worker.
  LOG(INFO) << "DummyScheduler scheduling";
  if (routines_.empty()) {
    LOG(INFO) << "No routine to schedule";
    return true;
  }
  for (auto &worker : workers_) {
    if (routines_.empty()) {
      break;
    }
    if (worker->isBusy()) {
      continue;
    }
    // First assign, then pop!
    worker->Assign(routines_.front());
    LOG(INFO) << "Assigning routine " << routines_.front()->get_id()
              << " to worker ";
    routines_.pop_front();
  }
  return true;
}

} // namespace gogort