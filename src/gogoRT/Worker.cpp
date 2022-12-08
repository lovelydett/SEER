//
// Created by Yuting Xie
// 22-12-7

#include "Worker.h"

#include <utility>

namespace gogort {

Worker::Worker()
    : inner_thread_(nullptr), is_running_(false), uuid_(get_next_uuid()) {}

bool Worker::Start(std::function<void()> func) {
  is_running_ = true;
  auto inner_func = [&]() {
    // Todo(yuting): set affinity and priority, fixed once set
    func();
  };
  inner_thread_ = std::make_unique<std::thread>(std::move(inner_func));
  // This worker dies here.
  return true;
}
bool Worker::Assign(std::shared_ptr<Routine> routine) {
  if (next_routine_ != nullptr) {
    return false;
  }
  next_routine_ = std::move(routine);
  return false;
}
bool Worker::isBusy() const { return next_routine_ != nullptr; }

bool Worker::Release() {
  next_routine_ = nullptr;
  return true;
}

} // namespace gogort