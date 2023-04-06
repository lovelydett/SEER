//
// Created by Yuting Xie on 03/04/2023.
//

#ifndef GOGO_PROJ_MONITOR_H
#define GOGO_PROJ_MONITOR_H

#include "TaskGraph.h"

#include <memory>

namespace gogort {

class Monitor {
private:
  static Monitor instance_;
  TaskGraph task_graph_;

private:
  Monitor();

public:
  static Monitor *Instance();
  TaskGraph *get_taskGraph();
};

} // namespace gogort

#endif // GOGO_PROJ_MONITOR_H
