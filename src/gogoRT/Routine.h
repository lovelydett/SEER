//
// Created by Yuting Xie on 03/12/2022.

#ifndef GOGO_ROUTINE_H
#define GOGO_ROUTINE_H

#include <functional>

namespace gogort {

class Routine {
public:
  Routine(std::function<void()>);
  // At the end of routine lifespan, it should clean up all message pointers it maintains
  ~Routine();
  bool Bind(std::function<void()>);
  bool Launch();
  bool Yield();

private:
  std::function<void()> func_;
};

} // namespace gogort

#endif // GOGO_ROUTINE_H