//
// Created by Yuting Xie on 03/12/2022.

#include "DummyTask.h"

namespace task {
bool DummyTask::Deal(const std::shared_ptr<DummyMessage> &) { return false; }
bool DummyTask::init_config(const std::string) { return false; }
} // namespace task