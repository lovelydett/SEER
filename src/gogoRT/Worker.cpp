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
  assert(routine != nullptr);
  assert(base_routine_ == nullptr);
  assert(preempt_routine_ == nullptr);
  base_routine_ = routine;
  return true;
}

bool Worker::is_preempting_() const { return is_preempt_; }

int Worker::get_core() const { return core_; }

Worker::Worker(Dispatcher &dispatcher, const int core)
    : base_thread_(nullptr), preempt_thread_(nullptr), is_preempt_(false),
      dispatcher_(dispatcher), status_(WORKER_IDLE), core_(core),
      preempt_ul_(preempt_lock_) {}

bool Worker::Start() {
  assert(base_thread_ == nullptr);
  assert(preempt_thread_ == nullptr);

  // The kernel executed by base_thread
  // Make sure no any IO operation in this kernel
  std::function<void(void)> base_kernel = [&]() {
    set_thread_affinity(pthread_self(), core_);
    set_thread_priority(pthread_self(), PRIORITY_MID);
    LOG(INFO) << "Base thread of worker " << get_id() << " is started";
    while (true) {
      // LOG(INFO) << "This line good!!! from " <<
      // get_thread_core(pthread_self());
      // set_thread_priority(pthread_self(), PRIORITY_MID);
      if (dispatcher_.AcquireSchedLock()) {
        // LOG(INFO) << "Get lock and do updating";
        dispatcher_.InvokeRoutine();
        dispatcher_.DoSchedule();
        dispatcher_.ReleaseSchedLock();
      }
      // Back to normal working priority
      // set_thread_priority(pthread_self(), PRIORITY_MID);
      if (base_routine_ != nullptr) {
        // LOG(INFO) << "Executing routine " << next_routine_->get_id();
        recorder->Append(base_routine_->get_task_name(), Recorder::kPoint,
                         base_routine_->get_id(), "execute_id");
        base_routine_->Run();
        if (base_routine_.use_count() != 1) {
          LOG(INFO) << "Wrong base routine use count: "
                    << base_routine_.use_count();
        }
        assert(base_routine_.use_count() == 1);
        base_routine_ = nullptr;
      }
    }
  };

  // The kernel executed by preempt_thread
  std::function<void(void)> preempt_kernel = [&]() {
    set_thread_affinity(pthread_self(), core_);
    set_thread_priority(pthread_self(), PRIORITY_HIGH);
    while (true) {
      preempt_cv_.wait(preempt_ul_);
      LOG(INFO) << "Preempt thread of worker " << get_id() << " is started";
      assert(preempt_routine_ != nullptr);
      recorder->Append(preempt_routine_->get_task_name(), Recorder::kPoint,
                       preempt_routine_->get_id(), "execute_preempt_id");
      preempt_routine_->Run();
      assert(preempt_routine_.use_count() == 1);
      preempt_routine_ = nullptr;
      is_preempt_ = false;
      // Back to low priority
      set_thread_priority(pthread_self(), PRIORITY_LOW);
    }
  };

  // Start the base thread and preempt thread kernels
  base_thread_ = std::make_unique<std::thread>(base_kernel);
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  preempt_thread_ = std::make_unique<std::thread>(preempt_kernel);

  LOG(INFO) << "Worker " << get_id() << " starts working";
  return true;
}

enum Worker::Status Worker::get_status() const { return status_; }

void Worker::Join() {
  if (base_thread_ != nullptr) {
    base_thread_->join();
  }
  if (preempt_thread_ != nullptr) {
    preempt_thread_->join();
  }
}

// Very critical, make sure this function call by another thread!
void Worker::RequestPreempt(std::shared_ptr<Routine> routine) {
  assert(preempt_routine_ == nullptr);
  assert(base_routine_ != nullptr); // Preempt should not happen when idle
  assert(base_thread_->get_id() != std::this_thread::get_id());
  preempt_routine_ = routine;
  is_preempt_ = true;
  // Notify the preempt thread
  preempt_cv_.notify_one();
}

uint32 Worker::get_id() const {
  auto id = base_thread_->get_id();
  return *(uint32 *)&id;
}

[[nodiscard]] bool Worker::is_idle() const {
  return base_routine_ == nullptr && preempt_routine_ == nullptr;
}

} // namespace gogort