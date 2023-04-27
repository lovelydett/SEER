//
// Created by Yuting Xie
// 23-4-28

#ifndef GOGO_PROJ_DISPATCHER_H
#define GOGO_PROJ_DISPATCHER_H

#include "Pipe.h"

#include <vector>

namespace pyxis {

class Dispatcher {
private:
  std::vector<Pipe> pipes_;
};

} // namespace pyxis

#endif // GOGO_PROJ_DISPATCHER_H
