//
// Created by Yuting Xie
// 2022/12/2
#include "Dispatcher.h"
#include "TaskFactory.h"
#include "Worker.h"
#include <array>

namespace gogort {

// Todo(yuting): load this from GFLAGS
static const int16 kMaxWorkerNum = 8;

Dispatcher::Dispatcher() {
  init_config();
}

bool Dispatcher::init_config() {
  // Mock for now, should be read from config file
  auto d_task = TaskFactory::Instance()->CreateTask("DummyTask", "");
  tasks_.insert({d_task->get_id(), d_task});
  task_name_to_id_.insert({d_task->get_name(), d_task->get_id()});
  return true;
}

bool Dispatcher::init_comm() {
  comm_buffer_ = CommBuffer::Instance();
  return comm_buffer_ != nullptr;
}

bool Dispatcher::Run() {
  // 1. Init worker pool
  std::array<Worker, kMaxWorkerNum> workers;

  // 2. Launch workers
  // 3. Join workers
}

} // namespace gogort