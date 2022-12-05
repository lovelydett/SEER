//
// Created by Yuting Xie on 03/12/2022.

#ifndef GOGO_ROUTINE_H
#define GOGO_ROUTINE_H

#include <functional>

namespace gogort {

class Routine {
public:
  Routine(std::function<void()>);
  ~Routine();
  bool bind(std::function<void()>);
  bool launch();
  bool yield();

private:
  std::function<void()> func_;
};

} // namespace gogort

#endif // GOGO_ROUTINE_H
