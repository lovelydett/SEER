//
// Created by Yuting Xie
// 22-12-7

#include "Worker.h"

namespace gogort {

Worker::Worker()
    : inner_thread_(nullptr), is_running_(false), uuid_(get_next_uuid()) {}

bool Worker::Start(std::function<void()> func) {
  is_running_ = true;
  inner_thread_ = std::make_unique<std::thread>(std::move(func));
  // This worker dies here.
  return true;
}

std::vector<Routine> &Worker::get_wait_list() { return wait_list_; }

} // namespace gogort