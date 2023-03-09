//
// Created by Yuting Xie on 18/02/2023.
//

#include "DynamicScheduler.h"
#include "../SchedulerFactory.h"

#include <cassert>
#include <yaml-cpp/yaml.h>

namespace gogort {

DynamicScheduler::DynamicScheduler(
    std::vector<std::shared_ptr<Worker>> &workers)
    : workers_(workers) {
  init_config("../../../config/scheduler/dynamic_scheduler.yaml");
}

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
  return inner_do_once(num_idle);
}

bool DynamicScheduler::inner_do_once(const int num_idle_worker) {
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

bool DynamicScheduler::init_config(const std::string config_file) {
  auto config = YAML::LoadFile(config_file);
  std::string base_scheduler_name = config["base_scheduler"].as<std::string>();
  std::string base_scheduler_config =
      config["base_scheduler_config"].as<std::string>();
  base_scheduler_ = SchedulerFactory::Instance()->CreateScheduler(
      base_scheduler_name, base_scheduler_config, workers_);

  assert(base_scheduler_ != nullptr);

  return true;
}

} // namespace gogort