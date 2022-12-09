//
// Created by Yuting Xie on 03/12/2022.

#ifndef GOGO_ROUTINE_H
#define GOGO_ROUTINE_H

#include "utils/utils.h"
#include <functional>

namespace gogort {

class Routine {
private:
  bool is_finished_;
  gogo_id_t id_;
  std::function<void()> func_;

public:
  explicit Routine(std::function<void()>);
  ~Routine();
  bool Run();
  [[nodiscard]] gogo_id_t get_id() const;
};

} // namespace gogort

#endif // GOGO_ROUTINE_H