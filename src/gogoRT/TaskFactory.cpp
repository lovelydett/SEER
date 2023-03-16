//
// Created by Yuting Xie
// 22-12-7

#include "TaskFactory.h"
#include "../task/DummyTask.h"
#include "../task/DummyTask2.h"
#include "../task/DummyTask3.h"

#include <glog/logging.h>

namespace gogort {

TaskFactory *TaskFactory::instance_ = nullptr;

TaskFactory *TaskFactory::Instance() {
  if (instance_ == nullptr) {
    instance_ = new TaskFactory();
  }
  return instance_;
}

std::shared_ptr<TaskBase>
TaskFactory::CreateTask(const std::string type, const std::string name,
                        const std::string config_path,
                        std::vector<std::string> &&in_pipes,
                        std::vector<std::string> &&out_pipes) {
  if (type == "DummyTask") {
    return std::make_shared<task::DummyTask>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else if (type == "DummyTask2") {
    return std::make_shared<task::DummyTask2>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else if (type == "DummyTask3") {
    return std::make_shared<task::DummyTask3>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else {
    LOG(ERROR) << "Task " << type << " not found!";
    return nullptr;
  }
  return nullptr;
}

} // namespace gogort