//
// Created by Yuting Xie on 09/01/2023.
//

#include "PriorityScheduler.h"
#include "../Worker.h"
#include "../utils/Recorder.h"

namespace gogort {

static Recorder *recorder = Recorder::Instance();

PriorityScheduler::PriorityScheduler(
    std::vector<std::shared_ptr<Worker>> &workers) {
  for (const auto &worker : workers) {
    // workers_[worker->get_priority()].emplace_back(worker);
    workers_.push_back(worker);
  }
  assert(!workers_.empty());
}

bool PriorityScheduler::DoOnce() {
  assert(!workers_.empty());
  // Yuting@2023-03-23: For now we just dont consider OS thread priority
  //  const auto num_routines = routines_.size();
  //  for (int i = 0; i < num_routines; ++i) {
  //    auto routine = routines_.top();
  //    uint16 priority = routine->get_priority();
  //    // Look for usable worker
  //    while (routine != nullptr && priority > 0) {
  //      auto it = workers_.find(routine->get_priority());
  //      assert(it != workers_.end());
  //      for (const auto &worker : it->second) {
  //        if (!worker->isBusy()) {
  //          worker->Assign(routine);
  //          routine = nullptr;
  //          break;
  //        }
  //      }
  //      priority--;
  //    }
  //    routines_.pop();
  //  }

  auto assign = [&](const std::shared_ptr<Worker> &worker) {
    if (worker->isBusy()) {
      return;
    }

    while (!routines_.empty()) {
      auto routine = routines_.top();
      routines_.pop();
      if (!routine->Expire()) {
        worker->Assign(routine);
        recorder->Append(routine->get_task_name(), Recorder::kPoint,
                         routine->get_id(), "assign_id");
        return;
      }
    }

    return;
  };

  std::for_each(workers_.begin(), workers_.end(), assign);

  return true;
}
bool PriorityScheduler::AddRoutine(const std::shared_ptr<Routine> routine) {
  routines_.push(routine);
  return false;
}

// Equivalent to std::less<T>, and remember priority_queue is big-top heap!
bool RoutineCmp::operator()(const RoutinePtr &routine1,
                            const RoutinePtr &routine2) {
  // Smaller number means higher priority
  return routine1->get_priority() > routine2->get_priority();
}
} // namespace gogort