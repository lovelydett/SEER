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
    auto routine = routines_.front();
    routines_.pop_front();
    worker->Assign(routine);
    LOG(INFO) << "Assign routine " << routine->get_id() << " to worker ";
  }
  return true;
}

} // namespace gogort