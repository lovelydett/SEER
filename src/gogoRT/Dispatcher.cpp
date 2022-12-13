//
// Created by Yuting Xie
// 2022/12/2
#include "Dispatcher.h"
#include "../messages/DummyMessage.h"
#include "../tasks/DummyTask.h"
#include "Invoker.h"
#include "SchedulerFactory.h"
#include "TaskFactory.h"
#include "Worker.h"

#include <cassert>

namespace gogort {

std::shared_ptr<Dispatcher> Dispatcher::instance_ = nullptr;

Dispatcher::Dispatcher() : scheduler_(nullptr) {
  init_workers();
  init_config();
  init_comm();
  init_tasks();
}

bool Dispatcher::init_workers() {
  const int num_workers = 1; // set num of workers as 1 to debug
  workers_.reserve(num_workers);
  // Mock for now
  for (int i = 0; i < num_workers; i++) {
    workers_.emplace_back(std::make_shared<Worker>(*this));
  }
  return true;
}

bool Dispatcher::init_config() { return true; }

bool Dispatcher::init_tasks() {
  // Mock for now, should be read from config file
  auto d_task = TaskFactory::Instance()->CreateTask("DummyTask", "");
  id_to_task_.insert({d_task->get_task_id(), d_task});
  task_name_to_id_.insert({d_task->get_task_name(), d_task->get_task_id()});
  scheduler_ = SchedulerFactory::Instance()->CreateScheduler("DummyScheduler",
                                                             "", workers_);

  // Create tasks according to their inputs
  for (auto &[name, task] : id_to_task_) {
    invokers_.emplace_back(task->get_invoker());
  }

  return true;
}

bool Dispatcher::init_comm() {
  // comm_buffer_ = CommBuffer::Instance();
  // assert(comm_buffer_ != nullptr);
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
bool Dispatcher::DoSchedule() { return scheduler_->DoSchedule(); }
bool Dispatcher::UpdateRoutine() {
  // Todo(yuting): check for each task, whether there are enough inputs.
  for (auto &invoker : invokers_) {
    auto routine = invoker->Invoke();
    if (routine) {
      scheduler_->AddRoutine(routine);
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