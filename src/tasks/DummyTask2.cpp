//
// Created by Yuting Xie on 27/12/2022.
//

#include "DummyTask2.h"
#include "../gogoRT/Comm/CommBuffer.h"
#include <glog/logging.h>

namespace task {

DummyTask2::DummyTask2(const std::string task_name)
    : Task<>(task_name), count(0) {
  writer_ = gogort::AcquireWriter<DummyMessage2>("dummy_pipe_2");
}

bool DummyTask2::Deal() {
  // DummyTask2 simply do matmul
  LOG(INFO) << "DummyTask2::Deal() starts";
  auto out_msg = std::make_shared<DummyMessage2>();
  for (int i = 0; i < M_SIZE; ++i) {
    out_msg->bias[i] = i;
  }

  writer_->Publish(out_msg);
  LOG(INFO) << "DummyTask2::Deal() ends, total count: " << ++count << "\n";
  return true;
}
bool DummyTask2::init_config(const std::string) { return true; }

std::shared_ptr<gogort::InvokerBase> DummyTask2::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  // Todo(yuting): read frequency_ms from config
  return std::make_shared<gogort::Invoker<>>(
      static_cast<std::shared_ptr<Task<>>>(this), 50);
}

} // namespace task