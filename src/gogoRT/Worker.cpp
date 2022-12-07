//
// Created by Yuting Xie
// 22-12-7

#include "Worker.h"

namespace gogort {

bool Worker::BindAndStart(std::function<void()> func) {
  is_running_ = true;
  inner_thread_ = std::make_unique<std::thread>(std::move(func));
  // This worker dies here.
  return true;
}
Worker::Worker() : inner_thread_(nullptr), is_running_(false) {

}

} // namespace gogort