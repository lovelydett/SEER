// A worker is a thread that can be assigned to a routine.
// Created by Yuting Xie
// 22-12-7

#ifndef GOGO_WORKER_H
#define GOGO_WORKER_H

#include "Dispatcher.h"
#include "Routine.h"
#include "utils/utils.h"

#include <condition_variable>
#include <functional>
#include <memory>
#include <thread>
#include <unordered_set>

namespace gogort {

class Dispatcher;

class Worker {
  enum Priority {
    PRIORITY_LOW = 90,
    PRIORITY_MID = 91,
    PRIORITY_HIGH = 99,
  };

  enum Status {
    WORKER_IDLE = 0,
    WORKER_BASE = 1,
    WORKER_PREEMPT = 2,
  };

private:
  const int core_;
  std::unique_ptr<std::thread> base_thread_;
  std::unique_ptr<std::thread> preempt_thread_;

  // Lock and condition variable to awake the preemption thread
  std::mutex preempt_lock_;
  std::condition_variable preempt_cv_;
  std::unique_lock<std::mutex> preempt_ul_;

  bool is_preempt_ = false;

  std::shared_ptr<Routine> base_routine_;
  std::shared_ptr<Routine> preempt_routine_;

  Dispatcher &dispatcher_;
  Status status_;

public:
  Worker() = delete;
  Worker(Dispatcher &dispatcher, int core);
  Worker(const Worker &) = delete;
  Worker &operator=(const Worker &) = delete;
  Worker(Worker &&) = delete;
  ~Worker() = default;

  // The whole process of a worker happens here
  bool Start();

  // Other utils to manage current worker
  bool Assign(std::shared_ptr<Routine> routine);
  void Join();
  void RequestPreempt(std::shared_ptr<Routine> routine);

  [[nodiscard]] uint32 get_id() const;
  [[nodiscard]] Status get_status() const;
  [[nodiscard]] bool is_preempting_() const;
  [[nodiscard]] int get_core() const;

  [[nodiscard]] bool is_idle() const;
};

} // namespace gogort

#endif // GOGO_WORKER_H
