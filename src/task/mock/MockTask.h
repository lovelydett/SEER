//
// Created by Yuting Xie on 16/03/2023.
//

#ifndef GOGO_PROJ_MOCKTASK_H
#define GOGO_PROJ_MOCKTASK_H

#include "../../Comm/PipeWriter.h"
#include "../../Task.h"
#include "../../messages/DummyMessage.h"
#include "MockWorkload.h"

#include <memory>

using gogort::Task;
using message::DummyMessage;

namespace task {

// Mocking task with 0 input and 1 output
class MockTask_0_1 : public Task<> {
private:
  int count_;
  int expected_latency_ms_;
  bool init_config(std::string) override;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer_;
  std::shared_ptr<MockWorkload> workload_;

public:
  MockTask_0_1(const std::string &name, const std::string &config_path,
               std::vector<std::string> &&in_pipes,
               std::vector<std::string> &&out_pipes);
  bool Deal() override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

// Mocking task with 0 input and 2 output
class MockTask_0_2 : public Task<> {
private:
  int count_;
  int expected_latency_ms_;
  bool init_config(std::string) override;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer1_;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer2_;
  std::shared_ptr<MockWorkload> workload_;

public:
  MockTask_0_2(const std::string &name, const std::string &config_path,
               std::vector<std::string> &&in_pipes,
               std::vector<std::string> &&out_pipes);
  bool Deal() override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

// Mocking task with 1 input and 0 output
class MockTask_1_0 : public Task<DummyMessage> {
private:
  int count_;
  int expected_latency_ms_;
  bool init_config(std::string) override;
  std::shared_ptr<MockWorkload> workload_;

public:
  MockTask_1_0(const std::string &name, const std::string &config_path,
               std::vector<std::string> &&in_pipes,
               std::vector<std::string> &&out_pipes);
  bool Deal(std::shared_ptr<DummyMessage> msg) override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

// Mocking task with 1 input and 1 output
class MockTask_1_1 : public Task<DummyMessage> {
private:
  int count_;
  int expected_latency_ms_;
  bool init_config(std::string) override;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer_;
  std::shared_ptr<MockWorkload> workload_;

public:
  MockTask_1_1(const std::string &name, const std::string &config_path,
               std::vector<std::string> &&in_pipes,
               std::vector<std::string> &&out_pipes);
  bool Deal(std::shared_ptr<DummyMessage> msg) override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

// Mocking task with 1 input and 2 output
class MockTask_1_2 : public Task<DummyMessage> {
private:
  int count_;
  int expected_latency_ms_;
  bool init_config(std::string) override;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer1_;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer2_;
  std::shared_ptr<MockWorkload> workload_;

public:
  MockTask_1_2(const std::string &name, const std::string &config_path,
               std::vector<std::string> &&in_pipes,
               std::vector<std::string> &&out_pipes);
  bool Deal(std::shared_ptr<DummyMessage> msg) override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

// Mocking task with 2 input and 0 output
class MockTask_2_0 : public Task<DummyMessage, DummyMessage> {
private:
  int count_;
  int expected_latency_ms_;
  bool init_config(std::string) override;
  std::shared_ptr<MockWorkload> workload_;

public:
  MockTask_2_0(const std::string &name, const std::string &config_path,
               std::vector<std::string> &&in_pipes,
               std::vector<std::string> &&out_pipes);
  bool Deal(std::shared_ptr<DummyMessage> msg1,
            std::shared_ptr<DummyMessage> msg2) override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

// Mocking task with 2 input and 1 output
class MockTask_2_1 : public Task<DummyMessage, DummyMessage> {
private:
  int count_;
  int expected_latency_ms_;
  bool init_config(std::string) override;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer_;
  std::shared_ptr<MockWorkload> workload_;

public:
  MockTask_2_1(const std::string &name, const std::string &config_path,
               std::vector<std::string> &&in_pipes,
               std::vector<std::string> &&out_pipes);
  bool Deal(std::shared_ptr<DummyMessage> msg1,
            std::shared_ptr<DummyMessage> msg2) override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

// Mocking task with 2 input and 2 output
class MockTask_2_2 : public Task<DummyMessage, DummyMessage> {
private:
  int count_;
  int expected_latency_ms_;
  bool init_config(std::string) override;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer1_;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer2_;
  std::shared_ptr<MockWorkload> workload_;

public:
  MockTask_2_2(const std::string &name, const std::string &config_path,
               std::vector<std::string> &&in_pipes,
               std::vector<std::string> &&out_pipes);
  bool Deal(std::shared_ptr<DummyMessage> msg1,
            std::shared_ptr<DummyMessage> msg2) override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

// Mocking task with 3 input and 0 output
class MockTask_3_0 : public Task<DummyMessage, DummyMessage, DummyMessage> {
private:
  int count_;
  int expected_latency_ms_;
  bool init_config(std::string) override;
  std::shared_ptr<MockWorkload> workload_;

public:
  MockTask_3_0(const std::string &name, const std::string &config_path,
               std::vector<std::string> &&in_pipes,
               std::vector<std::string> &&out_pipes);
  bool Deal(std::shared_ptr<DummyMessage> msg1,
            std::shared_ptr<DummyMessage> msg2,
            std::shared_ptr<DummyMessage> msg3) override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

// Mocking task with 3 input and 1 output
class MockTask_3_1 : public Task<DummyMessage, DummyMessage, DummyMessage> {
private:
  int count_;
  int expected_latency_ms_;
  bool init_config(std::string) override;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer_;
  std::shared_ptr<MockWorkload> workload_;

public:
  bool Deal(std::shared_ptr<DummyMessage> msg1,
            std::shared_ptr<DummyMessage> msg2,
            std::shared_ptr<DummyMessage> msg3) override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

// Mocking task with 3 input and 2 output
class MockTask_3_2 : public Task<DummyMessage, DummyMessage, DummyMessage> {
private:
  int count_;
  int expected_latency_ms_;
  bool init_config(std::string) override;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer1_;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer2_;
  std::shared_ptr<MockWorkload> workload_;

public:
  bool Deal(std::shared_ptr<DummyMessage> msg1,
            std::shared_ptr<DummyMessage> msg2,
            std::shared_ptr<DummyMessage> msg3) override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

} // namespace task

#endif // GOGO_PROJ_MOCKTASK_H
