//
// Created by Yuting Xie on 03/12/2022.

#ifndef GOGO_ROUTINE_H
#define GOGO_ROUTINE_H

#include "utils/perf_utils.h"
#include "utils/utils.h"

#include <functional>
#include <string>

namespace gogort {

class Routine {
private:
  bool is_finished_;
  std::string task_name_;
  gogo_id_t id_;
  uint64_t time_spawn_ms_;
  std::function<void()> func_;
  uint16 priority_;
  PerfMonitor perf_monitor_;

public:
  Routine(std::function<void()>, std::string, uint16 priority = 1);
  ~Routine();
  bool Run();
  bool Expire();
  [[nodiscard]] gogo_id_t get_id() const;
  [[nodiscard]] std::string get_task_name() const;
  [[nodiscard]] uint16 get_priority() const;
  [[nodiscard]] bool is_finished() const;
  void add_wrapper(std::function<void()>, std::function<void()>);
};

} // namespace gogort

#endif // GOGO_ROUTINE_H