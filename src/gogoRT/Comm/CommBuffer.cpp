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

} // namespace gogort