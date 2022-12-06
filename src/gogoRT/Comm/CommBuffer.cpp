//
// Created by Yuting Xie
// 22-9-13

#include "CommBuffer.h"

namespace gogort {

CommBuffer *CommBuffer::instance_ = nullptr;
std::mutex CommBuffer::mtx_ = std::mutex();

bool CommBuffer::init_config(const std::string config_path) {
  // TODO: read config file and init pipes_
  is_init_ = true;
  return false;
}

CommBuffer *CommBuffer::Instance() {
  if (instance_ == nullptr) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (instance_ == nullptr) {
      instance_ = new CommBuffer();
    }
  }
  return instance_;
}

bool CommBuffer::RegisterTask(const TaskBase * const task, const std::string pipe_name) {
//  if (!is_init_) {
//    return false;
//  }
  if (pipes_.find(pipe_name) == pipes_.end()) {
    return false;
  }
  task->get_task_name();
}

} // namespace gogort