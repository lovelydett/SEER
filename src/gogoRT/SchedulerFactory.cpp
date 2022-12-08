//
// Created by Yuting Xie
// 22-12-8

#include "SchedulerFactory.h"
#include "schedulers/DummyScheduler.h"

namespace gogort {

SchedulerFactory *SchedulerFactory::instance_ = nullptr;

SchedulerFactory *SchedulerFactory::Instance() {
  if (instance_ == nullptr) {
    instance_ = new SchedulerFactory();
  }
  return instance_;
}
std::shared_ptr<Scheduler> SchedulerFactory::CreateScheduler(
    const std::string scheduler_name, const std::string config_path,
    std::vector<std::shared_ptr<Worker>> &workers) {
  if (scheduler_name == "DummyScheduler") {
    return std::make_shared<DummyScheduler>(workers);
  }
  return nullptr;
}
} // namespace gogort