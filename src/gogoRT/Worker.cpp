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
  if (next_routine_ != nullptr) {
    return false;
  }
  // next_routine_ = std::move(routine);
  next_routine_ = routine;
  return true;
}
bool Worker::isBusy() const { return next_routine_ != nullptr; }

Worker::Worker(Dispatcher &dispatcher)
    : inner_thread_(nullptr), is_running_(false), uuid_(get_next_uuid()),
      dispatcher_(dispatcher), worker_stage_(STAGE_NONE) {}

bool Worker::StartStateMachine() {
  assert(inner_thread_ == nullptr);
  // The state-machine actually executes the function below
  std::function<void()> state_machine = [&]() {
    worker_stage_ = STAGE_PENDING_COMM;
    while (true) {
      switch (worker_stage_) {
      case STAGE_PENDING_COMM: {
        LOG(INFO) << "Thread " << inner_thread_->get_id() << " pending comm";
        dispatcher_.UpdateRoutine();
        worker_stage_ = STAGE_PENDING_SCHED;
        break;
      }
      case STAGE_PENDING_SCHED: {
        LOG(INFO) << "Thread " << inner_thread_->get_id() << " pending sched";
        dispatcher_.DoSchedule();
        worker_stage_ = STAGE_PENDING_EXEC;
        break;
      }
      case STAGE_PENDING_EXEC: {
        LOG(INFO) << "Thread " << inner_thread_->get_id() << " pending exec";
        if (next_routine_ != nullptr) {
          LOG(INFO) << "Thread " << inner_thread_->get_id()
                    << " has a routine to run";
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
  LOG(INFO) << "Thread " << inner_thread_->get_id() << " starts state-machine";
  return true;
}

} // namespace gogort