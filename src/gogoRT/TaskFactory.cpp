//
// Created by Yuting Xie
// 22-12-7

#include "TaskFactory.h"
#include "../task/DummyTask.h"
#include "../task/DummyTask2.h"
#include "../task/DummyTask3.h"
#include "../task/mock/MockTask.h"

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
  } else if (type == "MockTask_0_1") {
    return std::make_shared<task::MockTask_0_1>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else if (type == "MockTask_0_2") {
    return std::make_shared<task::MockTask_0_2>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else if (type == "MockTask_0_2") {
    return std::make_shared<task::MockTask_0_2>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else if (type == "MockTask_1_0") {
    return std::make_shared<task::MockTask_1_0>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else if (type == "MockTask_1_1") {
    return std::make_shared<task::MockTask_1_1>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else if (type == "MockTask_1_2") {
    return std::make_shared<task::MockTask_1_2>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else if (type == "MockTask_2_0") {
    return std::make_shared<task::MockTask_2_0>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else if (type == "MockTask_2_1") {
    return std::make_shared<task::MockTask_2_1>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else if (type == "MockTask_2_2") {
    return std::make_shared<task::MockTask_2_2>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else if (type == "MockTask_3_0") {
    return std::make_shared<task::MockTask_3_0>(
        name, config_path, std::move(in_pipes), std::move(out_pipes));
  } else {
    LOG(ERROR) << "Task " << type << " not found!";
    return nullptr;
  }
  return nullptr;
}

} // namespace gogort