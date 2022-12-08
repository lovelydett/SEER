//
// Created by Yuting Xie on 03/12/2022.

#ifndef GOGO_ROUTINE_H
#define GOGO_ROUTINE_H

#include <functional>

namespace gogort {

class Routine {
private:
  bool is_finished_;
  std::function<void()> func_;

public:
  explicit Routine(std::function<void()>);
  ~Routine();
  bool Run();
};

} // namespace gogort

#endif // GOGO_ROUTINE_H