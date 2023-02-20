//
// Created by Yuting Xie on 18/02/2023.
//

#ifndef GOGO_PROJ_DYNAMICSCHEDULER_H
#define GOGO_PROJ_DYNAMICSCHEDULER_H

#include "../Comm/CommBuffer.h"
#include "../Scheduler.h"

#include <unordered_map>
#include <vector>

namespace gogort {

class DynamicScheduler : public Scheduler {
private:
  std::vector<std::shared_ptr<Risk>>

private:
  bool DoOnce() override;

public:
  explicit DynamicScheduler(std::vector<std::shared_ptr<Worker>> &);
  bool AddRoutine(std::shared_ptr<Routine> routine);
};

} // namespace gogort

#endif // GOGO_PROJ_DYNAMICSCHEDULER_H
