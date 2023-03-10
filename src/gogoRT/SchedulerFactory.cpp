//
// Created by Yuting Xie
// 22-12-8

#include "SchedulerFactory.h"
#include "schedulers/DummyScheduler.h"
#include "schedulers/DynamicScheduler.h"
#include "schedulers/PriorityScheduler.h"

#include <string>

namespace gogort {

SchedulerFactory *SchedulerFactory::instance_ = nullptr;

SchedulerFactory *SchedulerFactory::Instance() {
  if (instance_ == nullptr) {
    instance_ = new SchedulerFactory();
  }
  return instance_;
}
std::shared_ptr<Scheduler> SchedulerFactory::CreateScheduler(
    std::string scheduler_name, std::string config_path,
    std::vector<std::shared_ptr<Worker>> &workers) {
  if (scheduler_name == "DummyScheduler") {
    return std::make_shared<DummyScheduler>(workers);
  } else if (scheduler_name == "PriorityScheduler") {
    return std::make_shared<PriorityScheduler>(workers);
  } else if (scheduler_name == "DynamicScheduler") {
    return std::make_shared<DynamicScheduler>(workers);
  }
  return nullptr;
}
} // namespace gogort