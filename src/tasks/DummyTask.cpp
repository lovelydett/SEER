//
// Created by Yuting Xie on 03/12/2022.

#include "DummyTask.h"
#include "../gogoRT/Comm/CommBuffer.h"
#include <glog/logging.h>

namespace task {
DummyTask::DummyTask() : Task<DummyMessage>("DummyTask"), count(0) {
  writer_ = gogort::AcquireWriter<DummyMessage>("dummy_pipe");
}

bool DummyTask::Deal(const std::shared_ptr<DummyMessage> &msg) {
  // DummyTask simply do matmul
  LOG(INFO) << "DummyTask::Deal() starts";
  double res[M_SIZE];
  auto matmul = [&]() {
    for (int i = 0; i < M_SIZE; ++i) {
      res[i] = 0;
      for (int j = 0; j < M_SIZE; ++j) {
        res[i] += msg->matrix[i][j] * vec[j];
      }
    }
  };
  for (int i = 0; i < 10; ++i) {
    matmul();
  }

  writer_->Publish(std::make_shared<DummyMessage>());
  LOG(INFO) << "DummyTask::Deal() ends, total count: " << ++count;
  return true;
}
bool DummyTask::init_config(const std::string) { return true; }

std::shared_ptr<gogort::InvokerBase> DummyTask::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  return std::make_shared<gogort::Invoker<DummyMessage>>(
      static_cast<std::shared_ptr<Task<DummyMessage>>>(this),
      gogort::AcquireReader<DummyMessage>("dummy_pipe"));
}
} // namespace task