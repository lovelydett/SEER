//
// Created by Yuting Xie
// 2022/12/8

#include "DummyScheduler.h"
#include "../Worker.h"
#include "../utils/Recorder.h"

#include <glog/logging.h>

namespace gogort {

DummyScheduler::DummyScheduler(std::vector<std::shared_ptr<Worker>> &workers)
    : workers_(workers) {}

bool DummyScheduler::DoOnce() {
  // Simply pick the first routine for next worker.
  // LOG(INFO) << "DummyScheduler scheduling";
  if (routines_.empty()) {
    // LOG(INFO) << "No routine to schedule";
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
    auto routine = routines_.front();
    routines_.pop_front();
    auto task_name = routine->get_task_name();
    assert(latest_tasks_.contains(task_name));
    if (latest_tasks_[task_name] > routine->get_id()) {
      continue;
    }

    worker->Assign(routine);
    assert(routine.use_count() == 2);
    LOG(INFO) << "Assigning routine (" << routine->get_id() << ", " << task_name
              << ") to worker: " << worker->get_id();
    Recorder::Instance()->Append("DummyScheduler_do_assign", Recorder::kPoint,
                                 routine->get_id(), task_name);
  }
  return true;
}

bool DummyScheduler::AddRoutine(std::shared_ptr<Routine> routine) {
  assert(routine != nullptr);
  latest_tasks_[routine->get_task_name()] = routine->get_id();
  routines_.push_back(routine);
  return true;
}

} // namespace gogort