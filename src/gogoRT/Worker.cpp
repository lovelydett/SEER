//
// Created by Yuting Xie
// 22-12-7

#include "Worker.h"
#include "Dispatcher.h"
#include "utils/utils.h"
#include <cassert>
#include <glog/logging.h>
#include <utility>

namespace gogort {

bool Worker::Assign(std::shared_ptr<Routine> routine) {
  assert(next_routine_ == nullptr);
  next_routine_ = std::move(routine);
  // next_routine_ = routine;
  return true;
}
bool Worker::isBusy() const { return next_routine_ != nullptr; }

Worker::Worker(Dispatcher &dispatcher)
    : inner_thread_(nullptr), is_running_(false), dispatcher_(dispatcher),
      worker_stage_(STAGE_NONE) {}

bool Worker::StartStateMachine() {
  assert(inner_thread_ == nullptr);
  // The state-machine actually executes the function below
  std::function<void()> state_machine = [&]() {
    worker_stage_ = STAGE_PENDING_COMM;
    while (true) {
      switch (worker_stage_) {
        // Yuting@2022/12/10: Lock the whole dispatcher before doing sched.
      case STAGE_PENDING_COMM: {
        LOG(INFO) << "Pending comm";
        if (dispatcher_.AcquireSchedLock()) {
          dispatcher_.UpdateRoutine();
          dispatcher_.ReleaseSchedLock();
          LOG(INFO) << "Comm done";
        }
        worker_stage_ = STAGE_PENDING_SCHED;
        break;
      }
      case STAGE_PENDING_SCHED: {
        LOG(INFO) << "Pending sched";
        if (dispatcher_.AcquireSchedLock()) {
          dispatcher_.DoSchedule();
          dispatcher_.ReleaseSchedLock();
          LOG(INFO) << "Sched done";
        }
        worker_stage_ = STAGE_PENDING_EXEC;
        break;
      }
      case STAGE_PENDING_EXEC: {
        LOG(INFO) << "Pending exec";
        if (next_routine_ != nullptr) {
          LOG(INFO) << "Executing routine " << next_routine_->get_id();
          next_routine_->Run();
          next_routine_ = nullptr;
        }
        worker_stage_ = STAGE_PENDING_COMM;
        break;
      }
      default:
        assert(false);
      }
    }
  };
  inner_thread_ = std::make_unique<std::thread>(state_machine);
  LOG(INFO) << "Starts state-machine";
  return true;
}
void Worker::Join() {
  if (inner_thread_ != nullptr) {
    inner_thread_->join();
  }
}

uint32 Worker::get_id() const {
  auto id = inner_thread_->get_id();
  return *(uint32 *)&id;
}

} // namespace gogort