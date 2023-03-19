//
// Created by Yuting Xie
// 22-9-13

#include "CommBuffer.h"

namespace gogort {

std::shared_ptr<CommBuffer> CommBuffer::instance_ = nullptr;
std::mutex CommBuffer::mtx_ = std::mutex();

CommBuffer::CommBuffer() {}

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

bool CommBuffer::AddPipe(const std::string pipe_name) {
  if (name_to_pipe_.find(pipe_name) == name_to_pipe_.end()) {
    name_to_pipe_.insert({pipe_name, std::make_shared<Pipe>(pipe_name)});
  }
  return true;
}

} // namespace gogort