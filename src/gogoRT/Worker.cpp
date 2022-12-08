//
// Created by Yuting Xie
// 22-12-7

#include "Worker.h"
#include "Dispatcher.h"
#include <assert.h>
#include <utility>

namespace gogort {

bool Worker::Assign(std::shared_ptr<Routine> routine) {
  if (next_routine_ != nullptr) {
    return false;
  }
  next_routine_ = std::move(routine);
  return false;
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
        dispatcher_.UpdateRoutine();
        worker_stage_ = STAGE_PENDING_SCHED;
        break;
      }
      case STAGE_PENDING_SCHED: {
        dispatcher_.DoSchedule();
        worker_stage_ = STAGE_PENDING_EXEC;
        break;
      }
      case STAGE_PENDING_EXEC: {
        if (next_routine_ != nullptr) {
          next_routine_->Run();
          next_routine_ = nullptr;
        }
        worker_stage_ = STAGE_PENDING_COMM;
        break;
      }
      }
    }
  };
  inner_thread_ = std::make_unique<std::thread>(state_machine);
  return true;
}

} // namespace gogort