//
// Created by Yuting Xie on 03/12/2022.
//

#include "Routine.h"
#include <glog/logging.h>

#include <utility>

namespace gogort {
Routine::Routine(std::function<void()> func, const std::string task_name)
    : func_(std::move(func)), task_name_(task_name), id_(get_next_uuid()),
      is_finished_(false) {}
Routine::~Routine() { // LOG(INFO) << "Routine " << id_ << " is destroyed";
}
bool Routine::Run() {
  assert(is_finished_ == false);
  func_();
  is_finished_ = true;
  return true;
}

gogo_id_t Routine::get_id() const { return id_; }
std::string Routine::get_task_name() const { return task_name_; }
bool Routine::is_finished() const { return is_finished_; }

} // namespace gogort