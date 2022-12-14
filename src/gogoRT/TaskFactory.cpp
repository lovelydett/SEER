//
// Created by Yuting Xie
// 22-12-7

#include "TaskFactory.h"
#include "../tasks/DummyTask.h"

namespace gogort {

TaskFactory *TaskFactory::instance_ = nullptr;

TaskFactory *TaskFactory::Instance() {
  if (instance_ == nullptr) {
    instance_ = new TaskFactory();
  }
  return instance_;
}

std::shared_ptr<TaskBase>
TaskFactory::CreateTask(const std::string task_name,
                        const std::string config_path) {
  static int count = 0;
  if (task_name == "DummyTask") {
    return std::make_shared<task::DummyTask>(task_name +
                                             std::to_string(++count));
  }
  return nullptr;
}

} // namespace gogort