// A pipe is a named queue for a specified type of message.
// Created by Yuting Xie
// 22-12-5

#ifndef GOGO_PIPE_H
#define GOGO_PIPE_H

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

#include "../Message.h"

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
  Pipe(const std::string pipe_name) : pipe_name_(pipe_name) { init_config(); }

  // This function can be multi-threaded.
  bool Enqueue(std::shared_ptr<Message> &message) {
    // Todo(yuting): publish message to this pipe
    inner_msg_ = message;
    return true;
  }

  // This function can be multi-threaded.
  bool Dequeue() { return true; }

  // This function can be multi-threaded.
  std::shared_ptr<Message> Top() { return inner_msg_; }
};

} // namespace gogort

#endif // GOGO_PIPE_H
