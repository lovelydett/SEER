//
// Created by Yuting Xie
// 22-9-13

#ifndef GOGO_COMMBUFFER_H
#define GOGO_COMMBUFFER_H

#include <unordered_map>
#include <mutex>
#include <queue>
#include <string>
#include <memory>
#include <mutex>

#include "Pipe.h"
#include "../Task.h"

namespace gogort {

class CommBuffer {
private:
  CommBuffer() = default;
  bool init_config(const std::string config_path);

private:
  bool is_init_ = false;
  static CommBuffer *instance_;
  static std::mutex mtx_;
  // Name to pipe
  std::unordered_map<std::string, std::unique_ptr<Pipe>> pipes_;

public:
  CommBuffer *Instance();
  bool RegisterTask(const TaskBase * const task);
};

} // namespace gogort

#endif // GOGO_COMMBUFFER_H
