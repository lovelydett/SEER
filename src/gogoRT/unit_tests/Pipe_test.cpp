// Unit test for Pipe
// Created by Yuting Xie
// 22-12-6

#include "../../messages/DummyMessage.h"
#include "../Comm/Pipe.h"

#include <gtest/gtest.h>
#include <memory>

// Test class for Pipe
class PipeTest : public ::testing::Test {
protected:
  void SetUp() override {
    pipe_ = std::make_shared<gogort::Pipe>("TestedPipe");
  }

  void TearDown() override { pipe_ = nullptr; }

  std::shared_ptr<gogort::Pipe> pipe_;
};

TEST_F(PipeTest, Enqueue) {
  auto msg = std::make_shared<message::DummyMessage>();
  EXPECT_TRUE(pipe_->Enqueue(msg));
  EXPECT_NE(pipe_, nullptr);
}
TEST_F(PipeTest, TimeStamp) {
  auto msg = std::make_shared<message::DummyMessage>();
  const int N = 100;
  int last_timestamp = -1;
  for (int i = 0; i < N; ++i) {
    pipe_->Enqueue(msg);
    int time = pipe_->get_timestamp();
    EXPECT_GE(time, last_timestamp);
    last_timestamp = time;
  }
}
