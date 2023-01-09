//
// Created by Yuting Xie
// 22-12-8

#ifndef GOGORT_SCHEDULER_H
#define GOGORT_SCHEDULER_H

#include "Routine.h"
#include "utils/utils.h"
#include <list>
#include <memory>
#include <vector>

namespace gogort {

class Worker;

// Todo(yuting): load this from GFLAGS
// static const int16 kMaxWorkerNum = 8;

class Scheduler {
protected:
  bool is_init_ = false;
  // Yuting@2023.1.9: let actual schedulers decide what containers to use.
  // std::vector<std::shared_ptr<Worker>> &workers_;
  // std::list<std::shared_ptr<Routine>> routines_;

protected:
  Scheduler() = default;
  virtual bool DoOnce() = 0;

public:
  // This function should rearrange all worker's wait-list and sets their next.
  bool DoSchedule();
  virtual bool AddRoutine(std::shared_ptr<Routine> routine) = 0;
};

} // namespace gogort

#endif // GOGORT_SCHEDULER_H