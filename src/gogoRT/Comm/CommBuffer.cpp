//
// Created by Yuting Xie
// 22-9-13

#include "CommBuffer.h"

namespace gogort {

std::shared_ptr<CommBuffer> CommBuffer::instance_ = nullptr;
std::mutex CommBuffer::mtx_ = std::mutex();

CommBuffer::CommBuffer() { init_config(""); }

bool CommBuffer::init_config(const std::string config_path) {
  // TODO: read config file and init pipes_
  is_init_ = true;

  // Mock
  name_to_pipe_.insert({"dummy_pipe", std::make_shared<Pipe>("dummy_pipe")});
  return false;
}

std::shared_ptr<CommBuffer> CommBuffer::Instance() {
  if (instance_ == nullptr) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (instance_ == nullptr) {
      instance_ = std::shared_ptr<CommBuffer>(new CommBuffer());
    }
  }
  return instance_;
}

std::shared_ptr<Pipe> CommBuffer::GetPipe(const std::string pipe_name) {
  if (name_to_pipe_.find(pipe_name) == name_to_pipe_.end()) {
    return nullptr;
  }
  return name_to_pipe_[pipe_name];
}

} // namespace gogort