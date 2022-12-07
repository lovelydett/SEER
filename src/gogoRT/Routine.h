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
  Routine(std::function<void()>);
  // At the end of routine lifespan, it should clean up all message pointers it
  // maintains
  ~Routine();
};

} // namespace gogort

#endif // GOGO_ROUTINE_H