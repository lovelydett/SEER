//
// Created by Yuting Xie on 03/12/2022.

#include "DummyTask.h"
#include "../gogoRT/Comm/CommBuffer.h"

#include <glog/logging.h>
#include <yaml-cpp/yaml.h>

namespace task {

DummyTask::DummyTask(const std::string task_name, const std::string config_file)
    : Task<>(task_name), count(0) {
  writer_ = gogort::AcquireWriter<DummyMessage>("dummy_pipe_1");
  assert(writer_ != nullptr);
  assert(DummyTask::init_config(config_file) == true);
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
bool DummyTask::init_config(const std::string config_file) {
  YAML::Node config = YAML::LoadFile("../../configs/tasks/" + config_file);
  frequency_ms_ = config["frequency_ms"].as<int16>();
  priority_ = config["priority"].as<uint16>();
  affinities_.clear();
  const auto &&affinities = std::move(config["affinities"]);
  affinities_.reserve(affinities.size());
  for (auto core : affinities) {
    affinities_.insert(core.as<uint16>());
  }

  return true;
}

std::shared_ptr<gogort::InvokerBase> DummyTask::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(frequency_ms_ > 0);
  return std::make_shared<gogort::Invoker<>>(
      static_cast<std::shared_ptr<Task<>>>(this), frequency_ms_);
}

} // namespace task