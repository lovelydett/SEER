//
// Created by Yuting Xie
// 22-12-8

#include "Scheduler.h"

namespace gogort {

Scheduler::Scheduler(std::vector<std::shared_ptr<Worker>> &workers)
    : workers_(workers) {}

bool Scheduler::AddRoutine(std::shared_ptr<Routine> routine) {
  routines_.push_back(routine);
  return true;
}
bool Scheduler::DoSchedule() {
  if (mtx_scheduler_.try_lock()) {
    DoOnce();
    mtx_scheduler_.unlock();
  }
  return true;
}

} // namespace gogort