// Invoker is a series of class templates to keep the task-R-pipe relationship.
// An invoker creates a routine when inputs are ready for a task.
// Created by Yuting Xie on 12/12/2022.

#ifndef GOGO_PROJ_INVOKER_H
#define GOGO_PROJ_INVOKER_H

#include "Comm/PipeReader.h"
#include "Routine.h"
#include "Task.h"

#include <chrono>
#include <functional>
#include <memory>
#include <utility>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

// Todo(yuting): figure out use steady clock or system clock
// If on MAC
#ifdef __APPLE__
using time_point = std::chrono::steady_clock::time_point;
#else
using time_point = std::chrono::system_clock::time_point;
#endif

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

private:
  std::shared_ptr<Routine> InnerInvoke() override {
    auto msg0 = pipe0_->Read();
    auto msg1 = pipe1_->Read();
    auto msg2 = pipe2_->Read();
    auto msg3 = pipe3_->Read();
    if (msg0 && msg1 && msg2 && msg3) {
      auto &&routine_func = std::bind(&Task<MSG0, MSG1, MSG2, MSG3>::Deal,
                                      &task_, msg0, msg1, msg2, msg3);
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
  explicit Invoker(std::shared_ptr<Task<>> task, const int16 frequency_ms)
      : task_(std::move(task)), frequency_(frequency_ms) {
    last_invoke_time_point_ = high_resolution_clock::now();
  }

private:
  std::shared_ptr<Task<>> task_;
  const std::chrono::milliseconds frequency_;
  time_point last_invoke_time_point_;

private:
  std::shared_ptr<Routine> InnerInvoke() override {
    auto now_time_point = high_resolution_clock::now();
    auto time_elapsed = duration_cast<std::chrono::milliseconds>(
        now_time_point - last_invoke_time_point_);
    if (duration_cast<std::chrono::milliseconds>(time_elapsed) >= frequency_) {
      last_invoke_time_point_ = now_time_point;
      auto &&routine_func = std::bind(&Task<>::Deal, task_);
      return std::make_shared<Routine>(routine_func, task_->get_task_name(),
                                       task_->get_priority());
    }
    return nullptr;
  }
};

// One-input invoker template class
template <class MSG0>
class Invoker<MSG0, NullClass, NullClass, NullClass> : public InvokerBase {
private:
  std::shared_ptr<Task<MSG0>> task_;
  std::shared_ptr<PipeReader<MSG0>> pipe0_;

private:
  std::shared_ptr<Routine> InnerInvoke() override {
    auto msg0 = pipe0_->Read();
    if (msg0) {
      auto &&routine_func = std::bind(&Task<MSG0>::Deal, task_, msg0);
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
  std::shared_ptr<Task<MSG0, MSG1>> task_;
  std::shared_ptr<PipeReader<MSG0>> pipe0_;
  std::shared_ptr<PipeReader<MSG1>> pipe1_;

private:
  std::shared_ptr<Routine> InnerInvoke() override {
    auto msg0 = pipe0_->Read();
    auto msg1 = pipe1_->Read();
    if (msg0 && msg1) {
      auto &&routine_func =
          std::bind(&Task<MSG0, MSG1>::Deal, task_, msg0, msg1);
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
  std::shared_ptr<TaskBase> task_;
  std::shared_ptr<PipeReader<MSG0>> pipe0_;
  std::shared_ptr<PipeReader<MSG1>> pipe1_;
  std::shared_ptr<PipeReader<MSG2>> pipe2_;

private:
  std::shared_ptr<Routine> InnerInvoke() override {
    auto msg0 = pipe0_->Read();
    auto msg1 = pipe1_->Read();
    auto msg2 = pipe2_->Read();
    if (msg0 && msg1 && msg2) {
      auto &&routine_func =
          std::bind(&Task<MSG0, MSG1, MSG2>::Deal, task_, msg0, msg1, msg2);
      return std::make_shared<Routine>(routine_func, task_->get_task_name(),
                                       task_->get_priority());
    }
    return nullptr;
  }

public:
  Invoker(std::shared_ptr<TaskBase> task,
          std::shared_ptr<PipeReader<MSG0>> pipe0,
          std::shared_ptr<PipeReader<MSG1>> pipe1,
          std::shared_ptr<PipeReader<MSG2>> pipe2)
      : task_(std::move(task)), pipe0_(std::move(pipe0)),
        pipe1_(std::move(pipe1)), pipe2_(std::move(pipe2)) {}
  ~Invoker() override = default;
};

} // namespace gogort

#endif // GOGO_PROJ_INVOKER_H