//
// Created by Yuting Xie
// 23-4-28

#ifndef SEER_DISPATCHER_H
#define SEER_DISPATCHER_H

#include "Pipe.h"

#include <vector>

namespace pyxis {

class Dispatcher {
private:
  std::vector<Pipe> pipes_;

private:
  bool init_config();
};

} // namespace pyxis

#endif // SEER_DISPATCHER_H
