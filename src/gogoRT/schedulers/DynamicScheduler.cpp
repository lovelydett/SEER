//
// Created by Yuting Xie on 18/02/2023.
//

#include "DynamicScheduler.h"

namespace gogort {

DynamicScheduler::DynamicScheduler(
    std::vector<std::shared_ptr<Worker>> &workers)
    : workers_(workers) {}

bool DynamicScheduler::DoOnce() {
  // NORMAL scheduling.
  // Todo(yuting): use other scheduling plugins here.
  base_scheduler_->DoSchedule();

  // IF idle workers, handle risks.
  int num_idle = 0;
  for (auto &worker : workers_) {
    if (!worker->isBusy()) {
      num_idle++;
    }
  }
  if (num_idle == 0) {
    return true;
  }

  // Otherwise, do inner
  return InnerDoOnce(num_idle);
}

bool DynamicScheduler::InnerDoOnce(const int num_idle_worker) {
  // EXPIRE old handled risks.
  for (auto it = handled_instances_.begin(); it != handled_instances_.end();) {
    if ((*it)->IsExpired()) {
      it = handled_instances_.erase(it);
    } else {
      ++it;
    }
  }

  // MATCH existing risks.
  for (auto &instance : handled_instances_) {
    if (instance->Match()) {
      // Todo(yuting): apply the control command immediately.
      return true;
    }
  }

  // DETECT new risks.
  for (auto &risk : registered_risks_) {
    auto &&detected_instances = risk->Detect();
    for (auto &instance : detected_instances) {
      // Insert the new instance at right place to keep order
      auto it = pending_instances_.begin();
      while (it != pending_instances_.end() &&
             (*it)->IsMoreCriticalThan(instance)) {
        it++;
      }
      pending_instances_.insert(it, instance);
    }
  }

  // ASSIGN pending risks to workers
  for (auto &worker : workers_) {
    if (!worker->isBusy()) {
      auto &instance = pending_instances_.front();
      worker->Assign(instance->GetHandler());
      pending_instances_.pop_front();
    }
  }

  return false;
}

} // namespace gogort