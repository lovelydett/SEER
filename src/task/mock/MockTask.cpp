//
// Created by Yuting Xie on 16/03/2023.
//

#include "MockTask.h"
#include "../../Comm/CommBuffer.h"
#include "../../Invoker.h"

#include <cassert>
#include <memory>

namespace task {

// Mocking task with 0 input and 1 output
MockTask_0_1::MockTask_0_1(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<>(name, std::move(in_pipes), std::move(out_pipes)), count(0) {
  MockTask_0_1::init_config(config_path);
}
bool MockTask_0_1::init_config(std::string config_path) {
  count = 0;
  assert(out_pipe_names_.size() == 1);
  writer_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[0]);
  return true;
}
bool MockTask_0_1::Deal() {
  auto &&msg = std::make_shared<message::MockMessage>();
  count++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

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
    : Task<>(name, std::move(in_pipes), std::move(out_pipes)), count(0) {
  MockTask_0_2::init_config(config_path);
}
bool MockTask_0_2::init_config(std::string config_path) {
  count = 0;
  assert(out_pipe_names_.size() == 2);
  writer1_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[0]);
  writer2_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[1]);
  return true;
}
bool MockTask_0_2::Deal() {
  auto &&msg = std::make_shared<message::MockMessage>();
  count++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

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
    : Task<MockMessage>(name, std::move(in_pipes), std::move(out_pipes)),
      count(0) {
  MockTask_1_0::init_config(config_path);
}
bool MockTask_1_0::init_config(std::string config_path) {
  count = 0;
  return true;
}
bool MockTask_1_0::Deal(std::shared_ptr<message::MockMessage> msg) {
  count++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_1_0::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 1);
  return std::make_shared<gogort::Invoker<MockMessage>>(
      static_cast<std::shared_ptr<Task<MockMessage>>>(this),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[0]));
}

// Mocking task with 1 input and 1 output
MockTask_1_1::MockTask_1_1(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<MockMessage>(name, std::move(in_pipes), std::move(out_pipes)),
      count(0) {
  MockTask_1_1::init_config(config_path);
}
bool MockTask_1_1::init_config(std::string config_path) {
  count = 0;
  assert(out_pipe_names_.size() == 1);
  writer_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[0]);
  return true;
}
bool MockTask_1_1::Deal(std::shared_ptr<message::MockMessage> msg) {
  count++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

  writer_->Publish(msg);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_1_1::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 1);
  return std::make_shared<gogort::Invoker<MockMessage>>(
      static_cast<std::shared_ptr<Task<MockMessage>>>(this),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[0]));
}

// Mocking task with 1 input and 2 output
MockTask_1_2::MockTask_1_2(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<MockMessage>(name, std::move(in_pipes), std::move(out_pipes)),
      count(0) {
  MockTask_1_2::init_config(config_path);
}
bool MockTask_1_2::init_config(std::string config_path) {
  count = 0;
  assert(out_pipe_names_.size() == 2);
  writer1_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[0]);
  writer2_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[1]);
  return true;
}
bool MockTask_1_2::Deal(std::shared_ptr<message::MockMessage> msg) {
  count++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

  writer1_->Publish(msg);
  writer2_->Publish(msg);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_1_2::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 1);
  return std::make_shared<gogort::Invoker<MockMessage>>(
      static_cast<std::shared_ptr<Task<MockMessage>>>(this),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[0]));
}

// Mocking task with 2 input and 0 output
MockTask_2_0::MockTask_2_0(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<MockMessage, MockMessage>(name, std::move(in_pipes),
                                     std::move(out_pipes)),
      count(0) {
  MockTask_2_0::init_config(config_path);
}
bool MockTask_2_0::init_config(std::string config_path) {
  count = 0;
  return true;
}
bool MockTask_2_0::Deal(std::shared_ptr<message::MockMessage> msg1,
                        std::shared_ptr<message::MockMessage> msg2) {
  count++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_2_0::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 2);
  return std::make_shared<gogort::Invoker<MockMessage, MockMessage>>(
      static_cast<std::shared_ptr<Task<MockMessage, MockMessage>>>(this),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[1]));
}

// Mocking task with 2 input and 1 output
MockTask_2_1::MockTask_2_1(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<MockMessage, MockMessage>(name, std::move(in_pipes),
                                     std::move(out_pipes)),
      count(0) {
  MockTask_2_1::init_config(config_path);
}
bool MockTask_2_1::init_config(std::string config_path) {
  count = 0;
  assert(out_pipe_names_.size() == 1);
  writer_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[0]);
  return true;
}
bool MockTask_2_1::Deal(std::shared_ptr<message::MockMessage> msg1,
                        std::shared_ptr<message::MockMessage> msg2) {
  count++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

  writer_->Publish(msg1);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_2_1::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 2);
  return std::make_shared<gogort::Invoker<MockMessage, MockMessage>>(
      static_cast<std::shared_ptr<Task<MockMessage, MockMessage>>>(this),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[1]));
}

// Mocking task with 2 input and 2 output
MockTask_2_2::MockTask_2_2(const std::string &name,
                           const std::string &config_path,
                           std::vector<std::string> &&in_pipes,
                           std::vector<std::string> &&out_pipes)
    : Task<MockMessage, MockMessage>(name, std::move(in_pipes),
                                     std::move(out_pipes)),
      count(0) {
  MockTask_2_2::init_config(config_path);
}
bool MockTask_2_2::init_config(std::string config_path) {
  count = 0;
  assert(out_pipe_names_.size() == 2);
  writer1_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[0]);
  writer2_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[1]);
  return true;
}
bool MockTask_2_2::Deal(std::shared_ptr<message::MockMessage> msg1,
                        std::shared_ptr<message::MockMessage> msg2) {
  count++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

  writer1_->Publish(msg1);
  writer2_->Publish(msg2);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_2_2::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 2);
  return std::make_shared<gogort::Invoker<MockMessage, MockMessage>>(
      static_cast<std::shared_ptr<Task<MockMessage, MockMessage>>>(this),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[1]));
}

// Mocking task with 3 input and 0 output
bool MockTask_3_0::init_config(std::string config_path) {
  count = 0;
  return true;
}
bool MockTask_3_0::Deal(std::shared_ptr<message::MockMessage> msg1,
                        std::shared_ptr<message::MockMessage> msg2,
                        std::shared_ptr<message::MockMessage> msg3) {
  count++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_3_0::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 3);
  return std::make_shared<
      gogort::Invoker<MockMessage, MockMessage, MockMessage>>(
      static_cast<std::shared_ptr<Task<MockMessage, MockMessage, MockMessage>>>(
          this),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[1]),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[2]));
}

// Mocking task with 3 input and 1 output
bool MockTask_3_1::init_config(std::string config_path) {
  count = 0;
  assert(out_pipe_names_.size() == 1);
  writer_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[0]);
  return true;
}
bool MockTask_3_1::Deal(std::shared_ptr<message::MockMessage> msg1,
                        std::shared_ptr<message::MockMessage> msg2,
                        std::shared_ptr<message::MockMessage> msg3) {
  count++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

  writer_->Publish(msg1);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_3_1::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 3);
  return std::make_shared<
      gogort::Invoker<MockMessage, MockMessage, MockMessage>>(
      static_cast<std::shared_ptr<Task<MockMessage, MockMessage, MockMessage>>>(
          this),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[1]),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[2]));
}

// Mocking task with 3 input and 2 output
bool MockTask_3_2::init_config(std::string config_path) {
  count = 0;
  assert(out_pipe_names_.size() == 2);
  writer1_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[0]);
  writer2_ = gogort::AcquireWriter<message::MockMessage>(out_pipe_names_[1]);
  return true;
}
bool MockTask_3_2::Deal(std::shared_ptr<message::MockMessage> msg1,
                        std::shared_ptr<message::MockMessage> msg2,
                        std::shared_ptr<message::MockMessage> msg3) {
  count++;

  // Todo(yuting): mock the pre-defined computational load and memory access.

  writer1_->Publish(msg1);
  writer2_->Publish(msg2);
  return true;
}
std::shared_ptr<gogort::InvokerBase> MockTask_3_2::get_invoker() {
  auto comm_buffer = gogort::CommBuffer::Instance();
  assert(in_pipe_names_.size() >= 3);
  return std::make_shared<
      gogort::Invoker<MockMessage, MockMessage, MockMessage>>(
      static_cast<std::shared_ptr<Task<MockMessage, MockMessage, MockMessage>>>(
          this),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[0]),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[1]),
      gogort::AcquireReader<MockMessage>(in_pipe_names_[2]));
}

} // namespace task