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

  // IF idle workers, handle risks.
  bool any_idle = false;
  for (auto &worker : workers_) {
    if (!worker->isBusy()) {
      any_idle = true;
      break;
    }
  }
  if (!any_idle) {
    return true;
  }

  // EXPIRE old risks.
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
      pending_instances_.push_back(instance);
    }
  }

  return true;
}

} // namespace gogort