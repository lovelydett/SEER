//
// Created by Yuting Xie
// 22-9-13

#ifndef GOGO_TASK_H
#define GOGO_TASK_H

#include "utils/utils.h"
#include <memory>
#include <string>
#include <utility>

namespace gogort {

class InvokerBase;

// Represent null type, facilitate variant template arguments
class NullClass {};

// TaskBase
class TaskBase {
private:
  gogo_id_t id_;
  const std::string task_name_;

private:
  virtual bool init_config(std::string) = 0;

public:
  TaskBase() = delete;
  explicit TaskBase(std::string task_name)
      : task_name_(std::move(task_name)), id_(get_next_uuid()){};
  bool Init(const gogo_id_t id, const std::string &config_path) {
    if (id == 0) {
      return false;
    }
    id_ = id;
    return init_config(config_path);
  }
  [[nodiscard]] std::string get_task_name() const { return task_name_; }
  [[nodiscard]] gogo_id_t get_task_id() const { return id_; }
  [[nodiscard]] virtual std::shared_ptr<InvokerBase> get_invoker() = 0;
};

// The original template class that supports at most 4 types of input messages
template <class MSG0 = NullClass, class MSG1 = NullClass,
          class MSG2 = NullClass, class MSG3 = NullClass>
class Task : public TaskBase {
public:
  Task() = delete;
  Task(const std::string task_name) : TaskBase(task_name) {}
  virtual bool Deal(std::shared_ptr<MSG0>, std::shared_ptr<MSG1>,
                    std::shared_ptr<MSG2>, std::shared_ptr<MSG3>) = 0;
};

// Yuting@2022-12-6: not allow task to take 0 input for now.
// Yuting@2022-12-28: allow task to take 0 input now.
template <>
class Task<NullClass, NullClass, NullClass, NullClass> : public TaskBase {
protected:
  int16 frequency_ms_;

public:
  Task() = delete;
  // Set frequency to -1 before load it from config file
  explicit Task(const std::string task_name)
      : TaskBase(task_name), frequency_ms_(-1){};
  virtual bool Deal() = 0;
};

// Task that takes 1 message
template <class MSG0>
class Task<MSG0, NullClass, NullClass, NullClass> : public TaskBase {
public:
  Task() = delete;
  explicit Task(const std::string task_name) : TaskBase(task_name){};
  virtual bool Deal(std::shared_ptr<MSG0>) = 0;
};

// Task that takes 2 messages
template <class MSG0, class MSG1>
class Task<MSG0, MSG1, NullClass, NullClass> : public TaskBase {
public:
  Task() = delete;
  explicit Task(const std::string task_name) : TaskBase(task_name){};
  virtual bool Deal(std::shared_ptr<MSG0>, std::shared_ptr<MSG1>) = 0;
};

// Task that takes 3 messages
template <class MSG0, class MSG1, class MSG2>
class Task<MSG0, MSG1, MSG2, NullClass> : public TaskBase {
public:
  Task() = delete;
  explicit Task(const std::string task_name) : TaskBase(task_name){};
  virtual bool Deal(std::shared_ptr<MSG0>, std::shared_ptr<MSG1>,
                    std::shared_ptr<MSG2>) = 0;
};

} // namespace gogort

#endif // GOGO_TASK_H
