//
// Created by Yuting Xie on 18/02/2023.
//

#include "DynamicScheduler.h"
#include "../risk/RiskFactory.h"
#include "../utils/Recorder.h"

#include <cassert>
#include <glog/logging.h>
#include <yaml-cpp/yaml.h>

namespace gogort {

DynamicScheduler::DynamicScheduler(
    std::vector<std::shared_ptr<Worker>> &workers)
    : workers_(workers) {
  init_config("../../config/scheduler/dynamic_scheduler.yaml");
}

bool DynamicScheduler::DoOnce() {
  // NORMAL scheduling.
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
  // UPDATE all pending instances, check whether handled.
  for (auto it = pending_instances_.begin(); it != pending_instances_.end();) {
    if ((*it)->IsHandled()) {
      handled_instances_.push_back((*it));
      it = pending_instances_.erase(it);
    } else {
      ++it;
    }
  }

  // EXPIRE old handled risks.
  for (auto it = handled_instances_.begin(); it != handled_instances_.end();) {
    if ((*it)->IsExpired()) {
      it = handled_instances_.erase(it);
    } else {
      ++it;
    }
  }

  // MATCH existing risks.
  for (auto it = handled_instances_.begin(); it != handled_instances_.end();
       ++it) {
    if ((*it)->IsHandled() && (*it)->Match()) {
      auto &&control_msg = (*it)->GetReactiveControl();
      assert(control_msg != nullptr);
      // Todo(yuting): apply the control command immediately, now only log
      Recorder::Instance()->Append((*it)->get_risk_name(), Recorder::kPoint,
                                   (*it)->get_id(), "match");
      handled_instances_.erase(it);
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
      Recorder::Instance()->Append(instance->get_risk_name(), Recorder::kPoint,
                                   instance->get_id(), "detect");
    }
  }

  // ASSIGN pending risks to workers
  for (auto &worker : workers_) {
    if (pending_instances_.empty()) {
      break;
    }
    if (!worker->isBusy()) {
      auto instance = pending_instances_.front();
      worker->Assign(instance->GetHandler());
      // LOG(INFO) << "Assigning " << instance->get_risk_name() << " to worker "
      // << worker->get_id();
      handled_instances_.push_back(instance);
      pending_instances_.pop_front();
    }
  }

  return true;
}

bool DynamicScheduler::init_config(const std::string config_file) {
  auto &&config = YAML::LoadFile(config_file);
  auto &&base_scheduler_name = config["base_scheduler"].as<std::string>();
  auto &&base_scheduler_config =
      config["base_scheduler_config"].as<std::string>();
  base_scheduler_ = SchedulerFactory::Instance()->CreateScheduler(
      base_scheduler_name, base_scheduler_config, workers_);
  assert(base_scheduler_ != nullptr);

  auto risks_of_interest =
      config["registered_risks"].as<std::vector<std::string>>();
  auto risk_factory = RiskFactory::Instance();
  for (auto &risk_name : risks_of_interest) {
    auto risk = risk_factory->CreateRisk(risk_name);
    assert(risk != nullptr);
    registered_risks_.emplace_back(risk);
  }

  return true;
}
bool DynamicScheduler::AddRoutine(std::shared_ptr<Routine> routine) {
  return base_scheduler_->AddRoutine(routine);
}

} // namespace gogort