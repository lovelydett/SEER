//
// Created by Yuting Xie
// 22-12-7

#include "TaskFactory.h"
#include "../tasks/DummyTask.h"
#include "../tasks/DummyTask2.h"
#include "../tasks/DummyTask3.h"

#include <glog/logging.h>

namespace gogort {

TaskFactory *TaskFactory::instance_ = nullptr;

TaskFactory *TaskFactory::Instance() {
  if (instance_ == nullptr) {
    instance_ = new TaskFactory();
  }
  return instance_;
}

std::shared_ptr<TaskBase> TaskFactory::CreateTask(
    const std::string task_name, const std::string config_path,
    std::vector<std::string> &&in_pipes, std::vector<std::string> &&out_pipes) {
  static int count = 0;
  if (task_name == "DummyTask") {
    return std::make_shared<task::DummyTask>(
        task_name + "_" + std::to_string(++count), config_path,
        std::move(in_pipes), std::move(out_pipes));
  } else if (task_name == "DummyTask2") {
    return std::make_shared<task::DummyTask2>(
        task_name + "_" + std::to_string(++count), config_path,
        std::move(in_pipes), std::move(out_pipes));
  } else if (task_name == "DummyTask3") {
    return std::make_shared<task::DummyTask3>(
        task_name + "_" + std::to_string(++count), config_path,
        std::move(in_pipes), std::move(out_pipes));
  } else {
    LOG(ERROR) << "Task " << task_name << " not found!";
    return nullptr;
  }
  return nullptr;
}

} // namespace gogort