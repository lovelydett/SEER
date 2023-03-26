//
// Created by Yuting Xie
// 22-12-7

#include "Worker.h"
#include "Dispatcher.h"
#include "utils/Recorder.h"
#include "utils/utils.h"
#include <cassert>
#include <glog/logging.h>

namespace gogort {

static Recorder *recorder = Recorder::Instance();

bool Worker::Assign(std::shared_ptr<Routine> routine) {
  assert(next_routine_ == nullptr);
  next_routine_ = routine;
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
        if (dispatcher_.AcquireSchedLock()) {
          // LOG(INFO) << "Get lock and do updating";
          dispatcher_.InvokeRoutine();
          dispatcher_.ReleaseSchedLock();
        }
        worker_stage_ = STAGE_PENDING_SCHED;
        break;
      }
      case STAGE_PENDING_SCHED: {
        if (dispatcher_.AcquireSchedLock()) {
          // LOG(INFO) << "Get lock and do scheduling";
          dispatcher_.DoSchedule();
          dispatcher_.ReleaseSchedLock();
        }
        worker_stage_ = STAGE_PENDING_EXEC;
        break;
      }
      case STAGE_PENDING_EXEC: {
        if (next_routine_ != nullptr) {
          // LOG(INFO) << "Executing routine " << next_routine_->get_id();
          recorder->Append(next_routine_->get_task_name(), Recorder::kPoint,
                           next_routine_->get_id(), "execute_id");
          next_routine_->Run();
          assert(next_routine_.use_count() == 1);
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
  // If on Linux, set the thread affinity
#ifdef __linux__
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(0, &cpuset);
  int rc = pthread_setaffinity_np(inner_thread_->native_handle(),
                                  sizeof(cpu_set_t), &cpuset);
  if (rc != 0) {
    LOG(ERROR) << "Error calling pthread_setaffinity_np: " << rc << "\n";
  }
#endif
  // Worker id is tid, mod a relatively large number for display
  LOG(INFO) << "Tid = " << get_id() % 1000 << " starts state-machine";
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

uint16 Worker::get_priority() const { return priority_; }

void Worker::set_priority(const uint16 priority) { priority_ = priority; }

} // namespace gogort