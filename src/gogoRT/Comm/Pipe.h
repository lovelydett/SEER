// A pipe is a named queue for a specified type of message.
// Created by Yuting Xie
// 22-12-5

#ifndef GOGO_PIPE_H
#define GOGO_PIPE_H

#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>

#include "../Message.h"
#include "../utils/utils.h"

namespace gogort {

class Pipe {
private:
  const std::string pipe_name_;
  std::shared_ptr<Message>
      inner_msg_; // Yuting@2022-12-5: For now we only store 1 message.
  // std::queue<MSG> inner_queue_;
  std::mutex mtx_; // Now lock pipe

private:
  Pipe() = default;

public:
  explicit Pipe(const std::string pipe_name) : pipe_name_(pipe_name) {}
  // This function can be multi-threaded.
  bool Enqueue(std::shared_ptr<Message> message) {
    std::lock_guard<std::mutex> lockGuard(mtx_);
    inner_msg_ = message;
    return true;
  }
  // This function can be multi-threaded.
  bool Dequeue() { return true; }
  // This function can be multi-threaded.
  std::shared_ptr<Message> Top() {
    std::lock_guard<std::mutex> lockGuard(mtx_);
    return inner_msg_;
  }
  [[nodiscard]] time_t get_timestamp() const {
    if (inner_msg_ == nullptr) {
      return 0;
    }
    return inner_msg_->get_timestamp();
  }
  [[nodiscard]] std::string get_pipe_name() const { return pipe_name_; }
};

} // namespace gogort

#endif // GOGO_PIPE_H
