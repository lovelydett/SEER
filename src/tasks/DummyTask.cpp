//
// Created by Yuting Xie on 03/12/2022.

#include "DummyTask.h"
#include "../gogoRT/Comm/CommBuffer.h"
#include <glog/logging.h>
#include <random>

namespace task {
DummyTask::DummyTask(const std::string task_name)
    : Task<DummyMessage>(task_name), count(0) {
  writer_ = gogort::AcquireWriter<DummyMessage>("dummy_pipe");
}

bool DummyTask::Deal(const std::shared_ptr<DummyMessage> msg) {
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

  // Randomly decide whether to send the result back and the number of times
  // Use c++11 random number generator
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 2);
  auto times = dis(gen);

  for (int i = 0; i < times; ++i) {
    writer_->Publish(std::make_shared<DummyMessage>());
  }
  LOG(INFO) << "DummyTask::Deal() ends, total count: " << ++count << ", send "
            << times << " times";
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