//
// Created by Yuting Xie
// 2022/12/2
#include "Dispatcher.h"
#include "SchedulerFactory.h"
#include "TaskFactory.h"
#include "Worker.h"

#include <glog/logging.h>
#include <yaml-cpp/yaml.h>

namespace gogort {

std::shared_ptr<Dispatcher> Dispatcher::instance_ = nullptr;

Dispatcher::Dispatcher() : scheduler_(nullptr) { init_config(); }

bool Dispatcher::init_config() {
  YAML::Node config = YAML::LoadFile(
      "../../config/mock/mock_task_graph_1/task_graph_mock_1.yaml");
  // YAML::Node config = YAML::LoadFile("../../config/task_graph_example.yaml");
  LOG(INFO) << config["tasks"].size() << " tasks in the graph";
  // invokers_.reserve(config["tasks"].size());

  // Init task based on config file
  for (auto &&task : config["tasks"]) {
    auto &&task_type = task["type"].as<std::string>();
    auto &&task_name = task["name"].as<std::string>();
    auto &&config_file = task["config_file"].as<std::string>();
    auto &&in_pipe_names = task["in_pipe_names"].as<std::vector<std::string>>();
    auto &&out_pipe_names =
        task["out_pipe_names"].as<std::vector<std::string>>();
    LOG(INFO) << "Creating task " << task_name << " of type " << task_type;

    // First, make sure the pipes are created
    for (auto &&pipe : in_pipe_names) {
      CommBuffer::Instance()->AddPipe(pipe);
    }
    for (auto &&pipe : out_pipe_names) {
      CommBuffer::Instance()->AddPipe(pipe);
    }

    // Then create the task
    auto p_task = TaskFactory::Instance()->CreateTask(
        task_type, task_name, config_file, std::move(in_pipe_names),
        std::move(out_pipe_names));
    if (p_task == nullptr) {
      LOG(ERROR) << "Failed to create task: " << task_name;
      exit(0);
    }
    // MUST keep a copy of the task pointer!!!!!!!!!!
    id_to_task_[p_task->get_task_id()] = p_task;
    invokers_.push_back(p_task->get_invoker());
  }
  LOG(INFO) << "In total " << invokers_.size() << " tasks are created";
  // exit(-1);

  // Init workers based on config file
  const int num_workers = config["num_workers"].as<int>();
  const int priority_range = config["priority_range"].as<int>();
  int a[num_workers];
  a[0] = 0;
  // num_workers = 1; // Set num of workers to 1 to debug
  workers_.reserve(num_workers * priority_range);
  // Mock for now
  for (int i = 0; i < num_workers; ++i) {
    for (int j = 1; j <= priority_range; ++j) {
      auto &&worker = std::make_shared<Worker>(*this);
      worker->set_priority(j);
      workers_.emplace_back(worker);
    }
  }

  // Init scheduler based on config file
  auto &&scheduler_name = config["scheduler"].as<std::string>();
  scheduler_ = SchedulerFactory::Instance()->CreateScheduler(scheduler_name, "",
                                                             workers_);

  return true;
}

bool Dispatcher::Run() {
  // 1. Launch workers
  for (auto &worker : workers_) {
    worker->StartStateMachine();
  }
  // 2. Join workers
  JoinWorkers();
  return true;
}

std::shared_ptr<Dispatcher> Dispatcher::Instance() {
  if (instance_ == nullptr) {
    instance_ = std::shared_ptr<Dispatcher>(new Dispatcher());
  }
  return instance_;
}

bool Dispatcher::DoSchedule() {
  if (scheduler_ == nullptr) {
    LOG(INFO) << "Scheduler not yet initialized";
    return false;
  }
  return scheduler_->DoSchedule();
}

bool Dispatcher::UpdateRoutine() {
  int count = 0;
  for (auto &invoker : invokers_) {
    auto routine = invoker->Invoke();
    if (routine) {
      scheduler_->AddRoutine(routine);
      count++;
    }
  }

  return true;
}

void Dispatcher::JoinWorkers() {
  for (auto &worker : workers_) {
    worker->Join();
  }
}

bool Dispatcher::AcquireSchedLock() { return mtx_sched_.try_lock(); }

void Dispatcher::ReleaseSchedLock() { mtx_sched_.unlock(); }

} // namespace gogort