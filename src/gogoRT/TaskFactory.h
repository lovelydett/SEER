//
// Created by Yuting Xie
// 22-12-7

#ifndef GOGO_TASKFACTORY_H
#define GOGO_TASKFACTORY_H

#include "Task.h"
#include <memory>

namespace gogort {

class TaskFactory {
private:
  static TaskFactory *instance_;

private:
  TaskFactory() = default;

public:
  static TaskFactory *Instance();
  std::shared_ptr<TaskBase> CreateTask(const std::string task_name,
                                       const std::string config_path);
};

} // namespace gogort

#endif // GOGO_TASKFACTORY_H
