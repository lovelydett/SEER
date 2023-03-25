//
// Created by Yuting Xie on 16/03/2023.
//

#include "MockTask.h"
#include "../../Comm/CommBuffer.h"
#include "../../Invoker.h"

#include <cassert>
#include <memory>
#include <yaml-cpp/yaml.h>

namespace task {

// Mocking task with 0 input and 1 output
MockTask_0_1::MockTask_0_1(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<>(name, std::move(in_pipes), std::move(out_pipes)), count_(0),
      expected_latency_ms_(-1) {
  MockTask_0_1::init_config(config_path);
}
bool MockTask_0_1::init_config(std::string config_path) {
  count_ = 0;
  assert(out_pipe_names_.size() == 1);
  writer_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[0]);

  auto &&config = YAML::LoadFile("../../config/mock/" + config_path);
  frequency_ms_ = config["frequency_ms"].as<int>();
  expected_latency_ms_ = config["expected_latency_ms"].as<int>();

  // Todo(yuting): load workload type and io_load_ from config
  workload_ = std::make_shared<MonteCarloPiWorkload>(0.5);

  return true;
}
bool MockTask_0_1::Deal() {
  auto &&msg = std::make_shared<message::DummyMessage>();
  count_++;

  workload_->RunFor(expected_latency_ms_);

  writer_->Publish(msg);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_0_1::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(frequency_ms_ > 0);
  return std::make_shared<gogort::Invoker<>>(
      static_cast<std::shared_ptr<Task<>>>(this), frequency_ms_);
}

// Mocking task with 0 input and 2 output
MockTask_0_2::MockTask_0_2(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<>(name, std::move(in_pipes), std::move(out_pipes)), count_(0),
      expected_latency_ms_(-1) {
  MockTask_0_2::init_config(config_path);
}
bool MockTask_0_2::init_config(std::string config_path) {
  count_ = 0;
  assert(out_pipe_names_.size() == 2);
  writer1_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[0]);
  writer2_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[1]);

  auto &&config = YAML::LoadFile("../../config/mock/" + config_path);
  frequency_ms_ = config["frequency_ms"].as<int>();
  expected_latency_ms_ = config["expected_latency_ms"].as<int>();

  workload_ = std::make_shared<MonteCarloPiWorkload>(0.5);

  return true;
}
bool MockTask_0_2::Deal() {
  auto &&msg = std::make_shared<message::DummyMessage>();
  count_++;

  workload_->RunFor(expected_latency_ms_);

  writer1_->Publish(msg);
  writer2_->Publish(msg);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_0_2::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(frequency_ms_ > 0);
  return std::make_shared<gogort::Invoker<>>(
      static_cast<std::shared_ptr<Task<>>>(this), frequency_ms_);
}

// Mocking task with 1 input and 0 output
MockTask_1_0::MockTask_1_0(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<DummyMessage>(name, std::move(in_pipes), std::move(out_pipes)),
      count_(0), expected_latency_ms_(-1) {
  MockTask_1_0::init_config(config_path);
}
bool MockTask_1_0::init_config(std::string config_path) {
  count_ = 0;
  auto &&config = YAML::LoadFile("../../config/mock/" + config_path);
  expected_latency_ms_ = config["expected_latency_ms"].as<int>();
  workload_ = std::make_shared<MonteCarloPiWorkload>(0.5);
  return true;
}
bool MockTask_1_0::Deal(std::shared_ptr<message::DummyMessage> msg) {
  count_++;

  workload_->RunFor(expected_latency_ms_);

  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_1_0::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 1);
  return std::make_shared<gogort::Invoker<DummyMessage>>(
      static_cast<std::shared_ptr<Task<DummyMessage>>>(this),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[0]));
}

// Mocking task with 1 input and 1 output
MockTask_1_1::MockTask_1_1(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<DummyMessage>(name, std::move(in_pipes), std::move(out_pipes)),
      count_(0), expected_latency_ms_(-1) {
  MockTask_1_1::init_config(config_path);
}
bool MockTask_1_1::init_config(std::string config_path) {
  count_ = 0;
  assert(out_pipe_names_.size() == 1);
  writer_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[0]);

  auto &&config = YAML::LoadFile("../../config/mock/" + config_path);
  expected_latency_ms_ = config["expected_latency_ms"].as<int>();

  workload_ = std::make_shared<MonteCarloPiWorkload>(0.5);

  return true;
}
bool MockTask_1_1::Deal(std::shared_ptr<message::DummyMessage> msg) {
  count_++;

  workload_->RunFor(expected_latency_ms_);

  writer_->Publish(msg);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_1_1::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 1);
  return std::make_shared<gogort::Invoker<DummyMessage>>(
      static_cast<std::shared_ptr<Task<DummyMessage>>>(this),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[0]));
}

// Mocking task with 1 input and 2 output
MockTask_1_2::MockTask_1_2(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<DummyMessage>(name, std::move(in_pipes), std::move(out_pipes)),
      count_(0), expected_latency_ms_(-1) {
  MockTask_1_2::init_config(config_path);
}
bool MockTask_1_2::init_config(std::string config_path) {
  count_ = 0;
  assert(out_pipe_names_.size() == 2);
  writer1_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[0]);
  writer2_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[1]);

  auto &&config = YAML::LoadFile("../../config/mock/" + config_path);
  expected_latency_ms_ = config["expected_latency_ms"].as<int>();

  workload_ = std::make_shared<MonteCarloPiWorkload>(0.5);

  return true;
}
bool MockTask_1_2::Deal(std::shared_ptr<message::DummyMessage> msg) {
  count_++;

  workload_->RunFor(expected_latency_ms_);

  writer1_->Publish(msg);
  writer2_->Publish(msg);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_1_2::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 1);
  return std::make_shared<gogort::Invoker<DummyMessage>>(
      static_cast<std::shared_ptr<Task<DummyMessage>>>(this),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[0]));
}

// Mocking task with 2 input and 0 output
MockTask_2_0::MockTask_2_0(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<DummyMessage, DummyMessage>(name, std::move(in_pipes),
                                       std::move(out_pipes)),
      count_(0), expected_latency_ms_(-1) {
  MockTask_2_0::init_config(config_path);
}
bool MockTask_2_0::init_config(std::string config_path) {
  count_ = 0;
  auto &&config = YAML::LoadFile("../../config/mock/" + config_path);
  expected_latency_ms_ = config["expected_latency_ms"].as<int>();

  workload_ = std::make_shared<MonteCarloPiWorkload>(0.5);

  return true;
}
bool MockTask_2_0::Deal(std::shared_ptr<message::DummyMessage> msg1,
                        std::shared_ptr<message::DummyMessage> msg2) {
  count_++;

  workload_->RunFor(expected_latency_ms_);

  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_2_0::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 2);
  return std::make_shared<gogort::Invoker<DummyMessage, DummyMessage>>(
      static_cast<std::shared_ptr<Task<DummyMessage, DummyMessage>>>(this),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[1]));
}

// Mocking task with 2 input and 1 output
MockTask_2_1::MockTask_2_1(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<DummyMessage, DummyMessage>(name, std::move(in_pipes),
                                       std::move(out_pipes)),
      count_(0), expected_latency_ms_(-1) {
  MockTask_2_1::init_config(config_path);
}
bool MockTask_2_1::init_config(std::string config_path) {
  count_ = 0;
  assert(out_pipe_names_.size() == 1);
  writer_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[0]);
  auto &&config = YAML::LoadFile("../../config/mock/" + config_path);
  expected_latency_ms_ = config["expected_latency_ms"].as<int>();

  workload_ = std::make_shared<MonteCarloPiWorkload>(0.5);

  return true;
}
bool MockTask_2_1::Deal(std::shared_ptr<message::DummyMessage> msg1,
                        std::shared_ptr<message::DummyMessage> msg2) {
  count_++;

  workload_->RunFor(expected_latency_ms_);

  writer_->Publish(msg1);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_2_1::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 2);
  return std::make_shared<gogort::Invoker<DummyMessage, DummyMessage>>(
      static_cast<std::shared_ptr<Task<DummyMessage, DummyMessage>>>(this),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[1]));
}

// Mocking task with 2 input and 2 output
MockTask_2_2::MockTask_2_2(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<DummyMessage, DummyMessage>(name, std::move(in_pipes),
                                       std::move(out_pipes)),
      count_(0), expected_latency_ms_(-1) {
  MockTask_2_2::init_config(config_path);
}
bool MockTask_2_2::init_config(std::string config_path) {
  count_ = 0;
  assert(out_pipe_names_.size() == 2);
  writer1_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[0]);
  writer2_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[1]);

  auto &&config = YAML::LoadFile("../../config/mock/" + config_path);
  expected_latency_ms_ = config["expected_latency_ms"].as<int>();

  workload_ = std::make_shared<MonteCarloPiWorkload>(0.5);

  return true;
}
bool MockTask_2_2::Deal(std::shared_ptr<message::DummyMessage> msg1,
                        std::shared_ptr<message::DummyMessage> msg2) {
  count_++;

  workload_->RunFor(expected_latency_ms_);

  writer1_->Publish(msg1);
  writer2_->Publish(msg2);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_2_2::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 2);
  return std::make_shared<gogort::Invoker<DummyMessage, DummyMessage>>(
      static_cast<std::shared_ptr<Task<DummyMessage, DummyMessage>>>(this),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[1]));
}

// Mocking task with 3 input and 0 output
MockTask_3_0::MockTask_3_0(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<DummyMessage, DummyMessage, DummyMessage>(name, std::move(in_pipes),
                                                     std::move(out_pipes)),
      count_(0), expected_latency_ms_(-1) {
  MockTask_3_0::init_config(config_path);
}
bool MockTask_3_0::init_config(std::string config_path) {
  count_ = 0;
  auto &&config = YAML::LoadFile("../../config/mock/" + config_path);
  expected_latency_ms_ = config["expected_latency_ms"].as<int>();

  workload_ = std::make_shared<MonteCarloPiWorkload>(0.5);
  return true;
}
bool MockTask_3_0::Deal(std::shared_ptr<message::DummyMessage> msg1,
                        std::shared_ptr<message::DummyMessage> msg2,
                        std::shared_ptr<message::DummyMessage> msg3) {
  count_++;

  workload_->RunFor(expected_latency_ms_);

  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_3_0::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 3);
  return std::make_shared<
      gogort::Invoker<DummyMessage, DummyMessage, DummyMessage>>(
      static_cast<
          std::shared_ptr<Task<DummyMessage, DummyMessage, DummyMessage>>>(
          this),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[1]),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[2]));
}

// Mocking task with 3 input and 1 output
bool MockTask_3_1::init_config(std::string config_path) {
  count_ = 0;
  assert(out_pipe_names_.size() == 1);
  writer_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[0]);
  return true;
}
bool MockTask_3_1::Deal(std::shared_ptr<message::DummyMessage> msg1,
                        std::shared_ptr<message::DummyMessage> msg2,
                        std::shared_ptr<message::DummyMessage> msg3) {
  count_++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

  writer_->Publish(msg1);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_3_1::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 3);
  return std::make_shared<
      gogort::Invoker<DummyMessage, DummyMessage, DummyMessage>>(
      static_cast<
          std::shared_ptr<Task<DummyMessage, DummyMessage, DummyMessage>>>(
          this),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[1]),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[2]));
}

// Mocking task with 3 input and 2 output
bool MockTask_3_2::init_config(std::string config_path) {
  count_ = 0;
  assert(out_pipe_names_.size() == 2);
  writer1_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[0]);
  writer2_ = gogort::AcquireWriter<message::DummyMessage>(out_pipe_names_[1]);
  return true;
}
bool MockTask_3_2::Deal(std::shared_ptr<message::DummyMessage> msg1,
                        std::shared_ptr<message::DummyMessage> msg2,
                        std::shared_ptr<message::DummyMessage> msg3) {
  count_++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

  writer1_->Publish(msg1);
  writer2_->Publish(msg2);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_3_2::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 3);
  return std::make_shared<
      gogort::Invoker<DummyMessage, DummyMessage, DummyMessage>>(
      static_cast<
          std::shared_ptr<Task<DummyMessage, DummyMessage, DummyMessage>>>(
          this),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[1]),
      gogort::AcquireReader<DummyMessage>(in_pipe_names_[2]));
}

} // namespace task