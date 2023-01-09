//
// Created by Yuting Xie on 27/12/2022.
//

#include "DummyTask2.h"
#include "../gogoRT/Comm/CommBuffer.h"

#include <glog/logging.h>
#include <yaml-cpp/yaml.h>

namespace task {

DummyTask2::DummyTask2(const std::string task_name,
                       const std::string config_file)
    : Task<>(task_name), count(0) {
  writer_ = gogort::AcquireWriter<DummyMessage2>("dummy_pipe_2");
  assert(writer_ != nullptr);
  init_config(config_file);
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
bool DummyTask2::init_config(const std::string config_file) {
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

std::shared_ptr<gogort::InvokerBase> DummyTask2::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(frequency_ms_ > 0);
  return std::make_shared<gogort::Invoker<>>(
      static_cast<std::shared_ptr<Task<>>>(this), frequency_ms_);
}

} // namespace task