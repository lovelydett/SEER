//
// Created by Yuting Xie
// 2022/12/2
#include "Dispatcher.h"
#include "TaskFactory.h"

namespace gogort {

Dispatcher::Dispatcher() {
  // Mock for now
  TaskFactory::Instance()->CreateTask("DummyTask", "");
}

} // namespace gogort