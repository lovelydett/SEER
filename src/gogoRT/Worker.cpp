//
// Created by Yuting Xie
// 22-12-7

#include "Worker.h"
#include "Dispatcher.h"
#include <assert.h>
#include <utility>

namespace gogort {

Worker::Worker()
    : inner_thread_(nullptr), is_running_(false), uuid_(get_next_uuid()) {}

bool Worker::Assign(std::shared_ptr<Routine> routine) {
  if (next_routine_ != nullptr) {
    return false;
  }
  next_routine_ = std::move(routine);
  return false;
}
bool Worker::isBusy() const { return next_routine_ != nullptr; }

bool Worker::Start(Dispatcher &dispatcher) {
  assert(inner_thread_ == nullptr);
  inner_thread_ = std::make_unique<std::thread>([&]() {
    while (true) {
      // 1. Dispatcher do scheduling, this sets all routines
      dispatcher.DoSchedule();
      // 2. Check if current worker is assigned a routine
      if (next_routine_ != nullptr) {
        next_routine_->Run(); // The job gets done here
      }
      // 3. Update routines based on comm_buffer
      // 4. Release the routine
      next_routine_ = nullptr;
    }
  });
  return true;
}

} // namespace gogort