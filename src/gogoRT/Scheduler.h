//
// Created by Yuting Xie
// 2022/12/8
#ifndef GOGORT_SCHEDULER_H
#define GOGORT_SCHEDULER_H

#include "Routine.h"
#include "Worker.h"
#include <array>
#include <list>
#include <memory>

// Todo(yuting): load this from GFLAGS
static const int16 kMaxWorkerNum = 8;

namespace gogort {

class Scheduler {
protected:
  std::array<std::shared_ptr<Worker>, kMaxWorkerNum> workers_;
  std::list<std::shared_ptr<Routine>> routines_;

public:
  // This function should rearrange all worker's wait-list and sets their next.
  virtual bool DoOnce() = 0;
};

} // namespace gogort

#endif // GOGORT_SCHEDULER_H
