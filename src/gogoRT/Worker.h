// A worker is a thread that can be assigned to a routine.
// Created by Yuting Xie
// 22-12-7

#ifndef GOGO_WORKER_H
#define GOGO_WORKER_H

#include "Dispatcher.h"
#include "Routine.h"
#include "utils/utils.h"
#include <functional>
#include <memory>
#include <thread>
#include <unordered_set>

namespace gogort {

class Dispatcher;

class Worker {
  enum WORKER_STAGE {
    STAGE_NONE = 0,
    STAGE_PENDING_COMM,
    STAGE_PENDING_SCHED,
    STAGE_PENDING_EXEC,
  };

private:
  std::unique_ptr<std::thread> inner_thread_;
  std::unordered_set<int16> affinity_;
  bool is_running_;
  uint16 priority_;
  gogo_id_t uuid_;
  std::shared_ptr<Routine> next_routine_;
  Dispatcher &dispatcher_;
  WORKER_STAGE worker_stage_;

public:
  Worker() = delete;
  explicit Worker(Dispatcher &dispatcher);
  Worker(const Worker &) = delete;
  Worker &operator=(const Worker &) = delete;
  Worker(Worker &&) = delete;
  ~Worker() = default;

  // The whole process of a worker happens here
  bool StartStateMachine();

  // Other utils to manage current worker
  bool Assign(std::shared_ptr<Routine> routine);
  bool Release();
  bool isBusy() const;
  void Join();
};

} // namespace gogort

#endif // GOGO_WORKER_H
