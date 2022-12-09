//
// Created by Yuting Xie on 03/12/2022.

#include "DummyTask.h"
#include <glog/logging.h>

namespace task {
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
  // Todo(yuting): publish msg again
  LOG(INFO) << "DummyTask::Deal() ends";
  return true;
}
bool DummyTask::init_config(const std::string) { return true; }
} // namespace task