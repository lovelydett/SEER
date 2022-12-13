//
// Created by Yuting Xie
// 2022/12/2
#include "Dispatcher.h"
#include "../messages/DummyMessage.h"
#include "../tasks/DummyTask.h"
#include "SchedulerFactory.h"
#include "TaskFactory.h"
#include "Worker.h"
#include "Invoker.h"

#include <cassert>

namespace gogort {

std::shared_ptr<Dispatcher> Dispatcher::instance_ = nullptr;

Dispatcher::Dispatcher() : scheduler_(nullptr) {
  init_workers();
  init_config();
  init_comm();
}

bool Dispatcher::init_workers() {
  const int num_workers = 4; // set num of workers as 1 to debug
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
  int num_of_inputs = 1;
  std::string task_name = "DummyTask";
  std::vector<std::string> input_pipe_names = {"DummyPipe"};
  switch (num_of_inputs) {
  case 1: {
    auto task = TaskFactory::Instance()->CreateTask(task_name, "");
    // Todo(yuting): get pipe readers from commbuffer.
    GetInvoker(&task, )
    break;
  }
  default:
    assert(false);

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

  // Mocking below, should be deleted
  //  auto d_message = std::make_shared<message::DummyMessage>();
  //  auto d_task = std::make_shared<task::DummyTask>();
  //  scheduler_->AddRoutine(
  //      std::make_shared<Routine>([&]() { d_task->Deal(d_message); }));
  scheduler_->AddRoutine(std::make_shared<Routine>([&]() {
    std::make_shared<task::DummyTask>()->Deal(
        std::make_shared<message::DummyMessage>());
  }));
  // Mocking above

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