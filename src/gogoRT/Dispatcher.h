/* Dispatcher is the main character of Gogo for it supports the following:
 * 1. Hot-swappable tasks (by listening to a socket)
 * 2. Create routines (timer-task or event-driven tasks)
 * 3. Maintain a thread pool and conduct routine-thread mapping
 * */
// Created by Yuting Xie
// 2022/12/2

#ifndef GOGO_DISPATCHER_H
#define GOGO_DISPATCHER_H

#include "Comm/CommBuffer.h"
#include "Routine.h"
#include "Task.h"
#include "Worker.h"
#include <unordered_map>

namespace gogort {

class Dispatcher {
private:
  static Dispatcher *instance_;
  bool is_init_ = false;
  std::string config_path_;
  CommBuffer *comm_buffer_;
  std::unordered_map<uuid_t, std::shared_ptr<TaskBase>> id_to_task_;
  std::unordered_map<std::string, uuid_t> task_name_to_id_;
  std::unordered_map<uuid_t, std::unique_ptr<Worker>> id_to_worker_;

private:
  Dispatcher();
  Dispatcher(Dispatcher &&) = delete;
  Dispatcher &operator=(Dispatcher &&) = delete;
  bool init_config();
  bool load_tasks();
  bool init_comm();

  // Worker thread executes this callback
  void worker_callback();

public:
  static Dispatcher *Instance_();
  bool isInit() const;

  // The whole program happens here
  bool Run();
};

} // namespace gogort

#endif // GOGO_DISPATCHER_H
