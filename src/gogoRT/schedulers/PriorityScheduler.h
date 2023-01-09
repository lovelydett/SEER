//
// Created by Yuting Xie on 09/01/2023.
//

#ifndef GOGO_PROJ_PRIORITYSCHEDULER_H
#define GOGO_PROJ_PRIORITYSCHEDULER_H

#include "../Dispatcher.h"
#include "../Scheduler.h"

namespace gogort {

class PriorityScheduler : public Scheduler {

private:
  bool DoOnce() override;

public:
  explicit PriorityScheduler(std::vector<std::shared_ptr<Worker>> &);
};

} // namespace gogort

#endif // GOGO_PROJ_PRIORITYSCHEDULER_H
