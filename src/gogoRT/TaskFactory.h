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
  std::shared_ptr<TaskBase> CreateTask(const std::string type,
                                       const std::string name,
                                       const std::string config_path,
                                       std::vector<std::string> &&in_pipes,
                                       std::vector<std::string> &&out_pipes);
};

} // namespace gogort

#endif // GOGO_TASKFACTORY_H
