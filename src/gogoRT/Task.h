//
// Created by Yuting Xie
// 22-9-13

#ifndef GOGO_TASK_H
#define GOGO_TASK_H

#include <string>
#include <vector>
#include <unordered_map>

namespace gogort {

class Task {
public:
  Task() = delete;
  Task(std::string config_file) : config_file_(config_file) {}
  virtual init() = 0;
  virtual void do_once() = 0;
  virtual void set_inputs(const std::vector<void*> inputs) = 0;

private:
  std::string task_name_;
  std::string config_file_;
  std::unordered_map<std::string, std::string> configs_;
  std::vector<void*> inputs;
};

} // namespace gogort

#endif // GOGO_TASK_H
