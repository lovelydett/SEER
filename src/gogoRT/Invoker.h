// Invoker is a series of class templates to keep the task-R-pipe relationship.
// An invoker creates a routine when inputs are ready for a task.
// Created by Yuting Xie on 12/12/2022.

#ifndef GOGO_PROJ_INVOKER_H
#define GOGO_PROJ_INVOKER_H

#include "Comm/PipeReader.h"
#include "Routine.h"
#include "Task.h"

#include <functional>
#include <memory>

namespace gogort {

class InvokerBase {
public:
  virtual ~InvokerBase() = default;
  virtual std::shared_ptr<Routine> Invoke() = 0;
};

// An invoker handles tasks that take at least 1 input.
// Yuting@2022-12-12: Each task should provide an invoker.
template <class M0, class M1 = NullClass, class M2 = NullClass,
          class M3 = NullClass>
class Invoker : public InvokerBase {
private:
  Task<M0, M1, M2, M3> &task_;
  PipeReader<M0> &pipe0_;
  PipeReader<M1> &pipe1_;
  PipeReader<M2> &pipe2_;
  PipeReader<M3> &pipe3_;

public:
  Invoker(Task<M0, M1, M2, M3> &task, PipeReader<M0> &pipe0,
          PipeReader<M1> &pipe1, PipeReader<M2> &pipe2, PipeReader<M3> &pipe3)
      : task_(task), pipe0_(pipe0), pipe1_(pipe1), pipe2_(pipe2),
        pipe3_(pipe3) {}
  ~Invoker() override = default;

  // Todo(yuting): check the status of pipes, and bind the inputs to the task.
  std::shared_ptr<Routine> InnerInvoke() {
    auto msg0 = pipe0_.Read();
    auto msg1 = pipe1_.Read();
    auto msg2 = pipe2_.Read();
    auto msg3 = pipe3_.Read();
    if (msg0 && msg1 && msg2 && msg3) {
      return std::make_shared<Routine>(std::bind(
          &Task<M0, M1, M2, M3>::Deal, &task_, msg0, msg1, msg2, msg3));
    }
    return nullptr;
  }
};

} // namespace gogort

#endif // GOGO_PROJ_INVOKER_H
