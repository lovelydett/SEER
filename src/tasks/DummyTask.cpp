//
// Created by Yuting Xie on 03/12/2022.

#include "DummyTask.h"
#include "../gogoRT/Comm/CommBuffer.h"
#include <glog/logging.h>

namespace task {

DummyTask::DummyTask(const std::string task_name)
    : Task<>(task_name), count(0) {
  writer_ = gogort::AcquireWriter<DummyMessage>("dummy_pipe_1");
}

bool DummyTask::Deal() {
  // DummyTask simply do matmul
  LOG(INFO) << "DummyTask::Deal() starts";
  auto out_msg = std::make_shared<DummyMessage>();
  for (int i = 0; i < M_SIZE; ++i) {
    for (int j = 0; j < M_SIZE; ++j) {
      out_msg->matrix[i][j] = i * M_SIZE + j;
    }
  }

  writer_->Publish(out_msg);
  LOG(INFO) << "DummyTask::Deal() ends, total count: " << ++count << "\n";
  return true;
}
bool DummyTask::init_config(const std::string) { return true; }

std::shared_ptr<gogort::InvokerBase> DummyTask::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  return std::make_shared<gogort::Invoker<>>(
      static_cast<std::shared_ptr<Task<>>>(this), 30);
}

} // namespace task