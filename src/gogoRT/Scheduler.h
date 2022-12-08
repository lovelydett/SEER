//
// Created by Yuting Xie
// 22-12-8

#ifndef GOGORT_SCHEDULER_H
#define GOGORT_SCHEDULER_H

#include "Routine.h"
#include "Worker.h"
#include "utils/utils.h"
#include <list>
#include <memory>
#include <vector>

namespace gogort {

// Todo(yuting): load this from GFLAGS
// static const int16 kMaxWorkerNum = 8;

class Scheduler {
protected:
  bool is_init_ = false;
  std::vector<std::shared_ptr<Worker>> &workers_;
  std::list<std::shared_ptr<Routine>> routines_;

public:
  // This function should rearrange all worker's wait-list and sets their next.
  explicit Scheduler(std::vector<std::shared_ptr<Worker>> &);
  virtual bool DoOnce() = 0;
  virtual bool AddRoutine(std::shared_ptr<Routine> routine);
};

} // namespace gogort

#endif // GOGORT_SCHEDULER_H