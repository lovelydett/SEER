// Invoker is a series of class templates to keep the task-R-pipe relationship.
// An invoker creates a routine when inputs are ready for a task.
// Created by Yuting Xie on 12/12/2022.

#ifndef GOGO_PROJ_INVOKER_H
#define GOGO_PROJ_INVOKER_H

#include "Comm/PipeReader.h"
#include "Routine.h"
#include "Task.h"
#include "utils/time_utils.h"

#include <chrono>
#include <functional>
#include <memory>
#include <utility>

namespace gogort {

class InvokerBase {
private:
  virtual std::shared_ptr<Routine> InnerInvoke() = 0;

public:
  virtual ~InvokerBase() = default;
  std::shared_ptr<Routine> Invoke() { return this->InnerInvoke(); }
};

// Yuting@2022-12-12: Each task should provide an invoker.
template <class MSG0 = NullClass, class MSG1 = NullClass,
          class MSG2 = NullClass, class MSG3 = NullClass>
class Invoker : public InvokerBase {
private:
  std::shared_ptr<Task<MSG0, MSG1, MSG2, MSG3>> task_;
  std::shared_ptr<PipeReader<MSG0>> pipe0_;
  std::shared_ptr<PipeReader<MSG1>> pipe1_;
  std::shared_ptr<PipeReader<MSG2>> pipe2_;
  std::shared_ptr<PipeReader<MSG3>> pipe3_;
  std::shared_ptr<MSG0> msg0_ = nullptr;
  std::shared_ptr<MSG1> msg1_ = nullptr;
  std::shared_ptr<MSG2> msg2_ = nullptr;
  std::shared_ptr<MSG3> msg3_ = nullptr;

private:
  std::shared_ptr<Routine> InnerInvoke() override {
    msg0_ = (msg0_ == nullptr) ? pipe0_->Read() : msg0_;
    msg1_ = (msg1_ == nullptr) ? pipe1_->Read() : msg1_;
    msg2_ = (msg2_ == nullptr) ? pipe2_->Read() : msg2_;
    msg3_ = (msg3_ == nullptr) ? pipe3_->Read() : msg3_;

    if (msg0_ && msg1_ && msg2_ && msg3_) {
      auto &&routine_func = std::bind(&Task<MSG0, MSG1, MSG2, MSG3>::Deal,
                                      &task_, msg0_, msg1_, msg2_, msg3_);
      msg0_ = nullptr;
      msg1_ = nullptr;
      msg2_ = nullptr;
      msg3_ = nullptr;
      return std::make_shared<Routine>(routine_func, task_->get_task_name(),
                                       task_->get_priority());
    }
    return nullptr;
  }

public:
  Invoker(std::shared_ptr<Task<MSG0, MSG1, MSG2, MSG3>> task,
          std::shared_ptr<PipeReader<MSG0>> pipe0,
          std::shared_ptr<PipeReader<MSG1>> pipe1,
          std::shared_ptr<PipeReader<MSG2>> pipe2,
          std::shared_ptr<PipeReader<MSG3>> pipe3)
      : task_(std::move(task)), pipe0_(std::move(pipe0)),
        pipe1_(std::move(pipe1)), pipe2_(std::move(pipe2)), pipe3_(pipe3) {}
  ~Invoker() override = default;
};

// Zero-input invoker, essentially timer task
template <>
class Invoker<NullClass, NullClass, NullClass, NullClass> : public InvokerBase {
public:
  Invoker() = delete;
  explicit Invoker(std::shared_ptr<Task<>> task, const int16_t frequency_ms)
      : task_(task), frequency_(frequency_ms) {
    timer_.start();
  }

private:
  std::shared_ptr<Task<>> task_ = nullptr;
  const int16_t frequency_;
  Timer timer_;

private:
  std::shared_ptr<Routine> InnerInvoke() override {
    auto elapse_ms = timer_.get_ms();
    if (elapse_ms >= frequency_) {
      // auto routine_func = std::bind(&Task<>::Deal, task_);
      LOG(INFO) << "Invoking task: " << task_->get_task_name();
      timer_.start();
      auto routine = std::make_shared<Routine>(std::bind(&Task<>::Deal, task_),
                                               task_->get_task_name(),
                                               task_->get_priority());
      return routine;
    }
    return nullptr;
  }
};

// One-input invoker template class
template <class MSG0>
class Invoker<MSG0, NullClass, NullClass, NullClass> : public InvokerBase {
private:
  std::shared_ptr<Task<MSG0>> task_ = nullptr;
  std::shared_ptr<PipeReader<MSG0>> pipe0_ = nullptr;
  std::shared_ptr<MSG0> msg0_ = nullptr;

private:
  std::shared_ptr<Routine> InnerInvoke() override {
    msg0_ = (msg0_ == nullptr) ? pipe0_->Read() : msg0_;
    if (msg0_) {
      auto &&routine_func = std::bind(&Task<MSG0>::Deal, task_, msg0_);
      msg0_ = nullptr;
      LOG(INFO) << "Invoking task: " << task_->get_task_name();
      return std::make_shared<Routine>(routine_func, task_->get_task_name(),
                                       task_->get_priority());
    }
    return nullptr;
  }

public:
  Invoker(std::shared_ptr<Task<MSG0>> task,
          std::shared_ptr<PipeReader<MSG0>> pipe0)
      : task_(std::move(task)), pipe0_(std::move(pipe0)) {}
  ~Invoker() override = default;
};

// Two-input invoker template class
template <class MSG0, class MSG1>
class Invoker<MSG0, MSG1, NullClass, NullClass> : public InvokerBase {
private:
  std::shared_ptr<Task<MSG0, MSG1>> task_ = nullptr;
  std::shared_ptr<PipeReader<MSG0>> pipe0_ = nullptr;
  std::shared_ptr<PipeReader<MSG1>> pipe1_ = nullptr;
  std::shared_ptr<MSG0> msg0_ = nullptr;
  std::shared_ptr<MSG1> msg1_ = nullptr;

private:
  std::shared_ptr<Routine> InnerInvoke() override {
    msg0_ = (msg0_ == nullptr) ? pipe0_->Read() : msg0_;
    msg1_ = (msg1_ == nullptr) ? pipe1_->Read() : msg1_;
    if (msg0_ && msg1_) {
      auto &&routine_func =
          std::bind(&Task<MSG0, MSG1>::Deal, task_, msg0_, msg1_);
      msg0_ = nullptr;
      msg1_ = nullptr;
      LOG(INFO) << "Invoking task: " << task_->get_task_name();
      return std::make_shared<Routine>(routine_func, task_->get_task_name(),
                                       task_->get_priority());
    }
    return nullptr;
  }

public:
  Invoker(std::shared_ptr<Task<MSG0, MSG1>> task,
          std::shared_ptr<PipeReader<MSG0>> pipe0,
          std::shared_ptr<PipeReader<MSG1>> pipe1)
      : task_(std::move(task)), pipe0_(std::move(pipe0)),
        pipe1_(std::move(pipe1)) {}
  ~Invoker() override = default;
};

// Three-input invoker template class
template <class MSG0, class MSG1, class MSG2>
class Invoker<MSG0, MSG1, MSG2, NullClass> : public InvokerBase {
private:
  std::shared_ptr<Task<MSG0, MSG1, MSG2>> task_ = nullptr;
  std::shared_ptr<PipeReader<MSG0>> pipe0_ = nullptr;
  std::shared_ptr<PipeReader<MSG1>> pipe1_ = nullptr;
  std::shared_ptr<PipeReader<MSG2>> pipe2_ = nullptr;
  std::shared_ptr<MSG0> msg0_ = nullptr;
  std::shared_ptr<MSG1> msg1_ = nullptr;
  std::shared_ptr<MSG2> msg2_ = nullptr;

private:
  std::shared_ptr<Routine> InnerInvoke() override {
    msg0_ = (msg0_ == nullptr) ? pipe0_->Read() : msg0_;
    msg1_ = (msg1_ == nullptr) ? pipe1_->Read() : msg1_;
    msg2_ = (msg2_ == nullptr) ? pipe2_->Read() : msg2_;
    if (msg0_ && msg1_ && msg2_) {
      auto &&routine_func =
          std::bind(&Task<MSG0, MSG1, MSG2>::Deal, task_, msg0_, msg1_, msg2_);
      msg0_ = nullptr;
      msg1_ = nullptr;
      msg2_ = nullptr;
      LOG(INFO) << "Invoking task: " << task_->get_task_name();
      return std::make_shared<Routine>(routine_func, task_->get_task_name(),
                                       task_->get_priority());
    }
    return nullptr;
  }

public:
  Invoker(std::shared_ptr<Task<MSG0, MSG1, MSG2>> task,
          std::shared_ptr<PipeReader<MSG0>> pipe0,
          std::shared_ptr<PipeReader<MSG1>> pipe1,
          std::shared_ptr<PipeReader<MSG2>> pipe2)
      : task_(std::move(task)), pipe0_(std::move(pipe0)),
        pipe1_(std::move(pipe1)), pipe2_(std::move(pipe2)) {}
  ~Invoker() override = default;
};

} // namespace gogort

#endif // GOGO_PROJ_INVOKER_H