//
// Created by Yuting Xie
// 22-9-13

#ifndef GOGO_TASK_H
#define GOGO_TASK_H

#include "utils/utils.h"
#include <memory>
#include <string>

typedef uint64 task_id_t;

namespace gogort {

// Represent null type, facilitate variant template arguments
class NullClass {};

// TaskBase
class TaskBase {
private:
  task_id_t id_;
  const std::string task_name_;

private:
  virtual bool init_config(const std::string) = 0;

public:
  TaskBase() = delete;
  TaskBase(const std::string task_name) : task_name_(task_name), id_(-1){};
  bool Init(const task_id_t id, const std::string config_path) {
    if (id == -1) {
      return false;
    }
    id_ = id;
    return init_config(config_path);
  }
  std::string get_task_name() const { return task_name_; }
  task_id_t get_task_id() const { return id_; }
};

// The original template class that supports at most 4 types of input messages
template <class MSG0 = NullClass, class MSG1 = NullClass,
          class MSG2 = NullClass, class MSG3 = NullClass>
class Task : public TaskBase {
public:
  Task() = delete;
  Task(const std::string task_name) : TaskBase(task_name){};
  virtual bool Deal(const std::shared_ptr<MSG0> &,
                    const std::shared_ptr<MSG1> &,
                    const std::shared_ptr<MSG2> &,
                    const std::shared_ptr<MSG3> &) = 0;
};

// Yuting@2022-12-6: not allow task to take 0 input for now.
// template <> class Task<NullClass, NullClass, NullClass, NullClass> {
//  virtual bool Deal() = 0;
//};

// Task that takes 1 message
template <class MSG0>
class Task<MSG0, NullClass, NullClass, NullClass> : public TaskBase {
public:
  Task() = delete;
  Task(const std::string task_name) : TaskBase(task_name){};
  virtual bool Deal(const std::shared_ptr<MSG0> &) = 0;
};

// Task that takes 2 messages
template <class MSG0, class MSG1>
class Task<MSG0, MSG1, NullClass, NullClass> : public TaskBase {
public:
  Task() = delete;
  Task(const std::string task_name) : TaskBase(task_name){};
  virtual bool Deal(const std::shared_ptr<MSG0> &,
                    const std::shared_ptr<MSG1> &) = 0;
};

// Task that takes 3 messages
template <class MSG0, class MSG1, class MSG2>
class Task<MSG0, MSG1, MSG2, NullClass> : public TaskBase {
public:
  Task() = delete;
  Task(const std::string task_name) : TaskBase(task_name){};
  virtual bool Deal(const std::shared_ptr<MSG0> &,
                    const std::shared_ptr<MSG1> &,
                    const std::shared_ptr<MSG2> &) = 0;
};

} // namespace gogort

#endif // GOGO_TASK_H
