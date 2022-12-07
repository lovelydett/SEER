/* Dispatcher is the main character of Gogo for it supports the following:
 * 1. Hot-swappable tasks (by listening to a socket)
 * 2. Create routines (timer-task or event-driven tasks)
 * 3. Maintain a thread pool and conduct routine-thread mapping
 * */
// Created by Yuting Xie
// 2022/12/2

#ifndef GOGO_DISPATCHER_H
#define GOGO_DISPATCHER_H

#include "Task.h"
#include <unordered_map>

namespace gogort {

class Dispatcher {
private:
  static Dispatcher *instance_;
  bool is_init_ = false;
  std::unordered_map<task_id_t, std::shared_ptr<TaskBase>> tasks_;
  std::unordered_map<std::string, task_id_t> task_name_to_id_;

private:
  Dispatcher();
  Dispatcher(Dispatcher &&) = delete;
  Dispatcher &operator=(Dispatcher &&) = delete;
  bool init_config();
  bool load_tasks();
  bool init_comm();

public:
  static Dispatcher *Instance_();
  bool isInit() const;

  // The whole program happens here
  bool Run();
};

} // namespace gogort

#endif // GOGO_DISPATCHER_H
