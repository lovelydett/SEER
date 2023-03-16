/* Dispatcher is the main character of Gogo for it supports the following:
 * 1. Hot-swappable task (by listening to a socket)
 * 2. Create routines (timer-task or event-driven task)
 * 3. Maintain a thread pool and conduct routine-thread mapping
 * */
// Created by Yuting Xie
// 2022/12/2

#ifndef GOGO_DISPATCHER_H
#define GOGO_DISPATCHER_H

#include "Comm/CommBuffer.h"
#include "Invoker.h"
#include "Routine.h"
#include "SchedulerFactory.h"
#include "Task.h"
#include <mutex>
#include <unordered_map>

namespace gogort {

class Worker;
class Scheduler;

class Dispatcher {
private:
  static std::shared_ptr<Dispatcher> instance_;
  bool is_init_ = false;
  std::string config_path_;
  std::unordered_map<gogo_id_t, std::shared_ptr<TaskBase>> id_to_task_;
  std::unordered_map<std::string, gogo_id_t> task_name_to_id_;
  std::vector<std::shared_ptr<Worker>> workers_;
  std::shared_ptr<Scheduler> scheduler_;
  std::shared_ptr<CommBuffer> comm_buffer_;
  // When one worker is scheduling, nobody gets work.
  std::mutex mtx_sched_;
  // All invokers
  std::vector<std::shared_ptr<InvokerBase>> invokers_;

private:
  Dispatcher();
  bool init_config();

public:
  static std::shared_ptr<Dispatcher> Instance();
  Dispatcher(Dispatcher &&) = delete;
  Dispatcher &operator=(Dispatcher &&) = delete;

  // The whole program happens here
  bool Run();
  bool DoSchedule();
  bool UpdateRoutine();
  void JoinWorkers();
  bool AcquireSchedLock();
  void ReleaseSchedLock();
};

} // namespace gogort

#endif // GOGO_DISPATCHER_H
