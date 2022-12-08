//
// Created by Yuting Xie
// 22-12-8

#ifndef GOGORT_SCHEDULERFACTORY_H
#define GOGORT_SCHEDULERFACTORY_H

#include "Scheduler.h"
#include <memory>

namespace gogort {

class SchedulerFactory {
private:
  static SchedulerFactory *instance_;

public:
  static SchedulerFactory *Instance();
  std::shared_ptr<Scheduler>
  CreateScheduler(const std::string scheduler_name,
                  const std::string config_path,
                  std::vector<std::shared_ptr<Worker>> &workers);
};

} // namespace gogort

#endif // GOGORT_SCHEDULERFACTORY_H
