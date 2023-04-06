//
// Created by Yuting Xie on 03/04/2023.
//

#include "Monitor.h"

#include <cassert>

namespace gogort {

Monitor Monitor::instance_;

Monitor::Monitor() {}

Monitor *Monitor::Instance() { return &instance_; }

TaskGraph *Monitor::get_taskGraph() { return &task_graph_; }
} // namespace gogort