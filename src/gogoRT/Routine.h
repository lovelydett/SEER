//
// Created by Yuting Xie on 03/12/2022.

#ifndef GOGO_ROUTINE_H
#define GOGO_ROUTINE_H

#include "utils/utils.h"

#include <functional>
#include <string>

namespace gogort {

class Routine {
private:
  bool is_finished_;
  std::string task_name_;
  gogo_id_t id_;
  std::function<void()> func_;

public:
  Routine(std::function<void()>, std::string);
  ~Routine();
  bool Run();
  [[nodiscard]] gogo_id_t get_id() const;
  [[nodiscard]] std::string get_task_name() const;
  [[nodiscard]] bool is_finished() const;
};

} // namespace gogort

#endif // GOGO_ROUTINE_H