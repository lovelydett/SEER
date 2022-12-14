// A pipe is a named queue for a specified type of message.
// Created by Yuting Xie
// 22-12-5

#ifndef GOGO_PIPE_H
#define GOGO_PIPE_H

#include <glog/logging.h>
#include <memory>
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

private:
  Pipe() = default;
  bool init_config() {
    // Todo(yuting): read config file and init readers_
    return true;
  }

public:
  explicit Pipe(const std::string pipe_name) : pipe_name_(pipe_name) {
    LOG(INFO) << "Creating pipe: " << pipe_name_;
    init_config();
  }
  // This function can be multi-threaded.
  bool Enqueue(std::shared_ptr<Message> message) {
    inner_msg_ = std::move(message);
    return true;
  }
  // This function can be multi-threaded.
  bool Dequeue() { return true; }
  // This function can be multi-threaded.
  std::shared_ptr<Message> Top() { return inner_msg_; }
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
