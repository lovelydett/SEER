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
private:
  std::unique_ptr<std::thread> inner_thread_;
  std::unordered_set<int16> affinity_;
  bool is_running_;
  uint16 priority_;
  uuid_t uuid_;
  std::shared_ptr<Routine> next_routine_;

public:
  Worker();
  Worker(const Worker &) = delete;
  Worker &operator=(const Worker &) = delete;
  Worker(Worker &&) = delete;
  ~Worker() = default;

  bool Start(Dispatcher &dispatcher);
  bool Assign(std::shared_ptr<Routine> routine);
  bool Release();
  bool isBusy() const;
};

} // namespace gogort

#endif // GOGO_WORKER_H
