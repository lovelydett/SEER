//
// Created by Yuting Xie on 09/01/2023.
//

#include "PriorityScheduler.h"
#include "../Worker.h"

namespace gogort {

PriorityScheduler::PriorityScheduler(
    std::vector<std::shared_ptr<Worker>> &workers) {
  for (auto worker : workers) {
    workers_[worker->get_priority()].emplace_back(worker);
  }
  assert(workers_.size() > 0);
}

bool PriorityScheduler::DoOnce() {
  assert(workers_.size() > 0);
  const auto num_routines = routines_.size();
  for (int i = 0; i < num_routines; ++i) {
    auto routine = routines_.top();
    uint16 priority = routine->get_priority();
    // Look for usable worker
    while (routine != nullptr && priority > 0) {
      auto it = workers_.find(routine->get_priority());
      assert(it != workers_.end());
      for (const auto &worker : it->second) {
        if (!worker->isBusy()) {
          worker->Assign(routine);
          routine = nullptr;
          break;
        }
      }
      priority--;
    }
    routines_.pop();
  }

  return true;
}
bool PriorityScheduler::AddRoutine(const std::shared_ptr<Routine> routine) {
  routines_.emplace(routine);
  return false;
}

bool RoutineCmp::operator()(const RoutinePtr &routine1,
                            const RoutinePtr &routine2) {
  return routine1->get_priority() > routine2->get_priority();
}
} // namespace gogort