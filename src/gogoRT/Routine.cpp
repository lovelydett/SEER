//
// Created by Yuting Xie on 03/12/2022.
//

#include "Routine.h"

#include <cassert>
#include <glog/logging.h>
#include <utility>

namespace gogort {
Routine::Routine(std::function<void()> func, const std::string task_name,
                 const uint16 priority)
    : func_(func), task_name_(task_name), id_(get_next_uuid()),
      is_finished_(false), priority_(priority) {
  assert(priority < uint64(4));
}
Routine::~Routine() { // LOG(INFO) << "Routine " << id_ << " is destroyed";
}
bool Routine::Run() {
  assert(is_finished_ == false);
  perf_monitor_.start();
  func_();
  perf_monitor_.stop_and_record(task_name_);
  is_finished_ = true;
  return true;
}

gogo_id_t Routine::get_id() const { return id_; }
std::string Routine::get_task_name() const { return task_name_; }
bool Routine::is_finished() const { return is_finished_; }
uint16 Routine::get_priority() const { return priority_; }

// Wrapper can only be added, not removable
void Routine::add_wrapper(std::function<void()> before,
                          std::function<void()> after) {
  func_ = [&]() {
    before();
    func_();
    after();
  };
}

} // namespace gogort