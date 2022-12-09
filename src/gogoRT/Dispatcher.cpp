//
// Created by Yuting Xie
// 2022/12/2
#include "Dispatcher.h"
#include "../messages/DummyMessage.h"
#include "../tasks/DummyTask.h"
#include "SchedulerFactory.h"
#include "TaskFactory.h"
#include "Worker.h"
#include <array>
#include <chrono>

namespace gogort {

std::shared_ptr<Dispatcher> Dispatcher::instance_ = nullptr;

Dispatcher::Dispatcher() : scheduler_(nullptr) {
  init_workers();
  init_config();
  init_comm();
}

bool Dispatcher::init_workers() {
  workers_.reserve(4);
  // Mock for now
  for (int i = 0; i < 4; i++) {
    workers_.emplace_back(std::make_shared<Worker>(*this));
  }
  return true;
}

bool Dispatcher::init_config() {
  // Mock for now, should be read from config file
  auto d_task = TaskFactory::Instance()->CreateTask("DummyTask", "");
  id_to_task_.insert({d_task->get_task_id(), d_task});
  task_name_to_id_.insert({d_task->get_task_name(), d_task->get_task_id()});
  scheduler_ = SchedulerFactory::Instance()->CreateScheduler("DummyScheduler",
                                                             "", workers_);
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
  // Simply sleep for now
  std::this_thread::sleep_for(std::chrono::seconds(5));
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
  auto d_message = std::make_shared<message::DummyMessage>();
  auto d_task = std::make_shared<task::DummyTask>();
  scheduler_->AddRoutine(
      std::make_shared<Routine>([&]() { // d_task->Deal(d_message);
        int a = 1; }));
  // Mocking above

  return true;
}

} // namespace gogort