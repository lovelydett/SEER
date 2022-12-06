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
  // Pipe name to pipe
  std::unordered_map<std::string, std::unique_ptr<Pipe>> name_to_pipe_;
  // Task name to pipe name
  std::unordered_map<std::string, std::string> task_to_pipe_;
  // Task name to task
  std::unordered_map<std::string, TaskBase*> name_to_task_;

public:
  CommBuffer *Instance();
  // CommBuffer will prepare the messages in pipe-of-interest for the task.
  bool RegisterTask(const TaskBase * const task, const std::string pipe_name);
};

} // namespace gogort

#endif // GOGO_COMMBUFFER_H
