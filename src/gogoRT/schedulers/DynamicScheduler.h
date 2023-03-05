//
// Created by Yuting Xie on 18/02/2023.
//

#ifndef GOGO_PROJ_DYNAMICSCHEDULER_H
#define GOGO_PROJ_DYNAMICSCHEDULER_H

#include "../Comm/CommBuffer.h"
#include "../Scheduler.h"
#include "../Worker.h"
#include "../risk/Risk.h"
#include "../risk/RiskInstance.h"

#include <list>
#include <unordered_map>
#include <vector>

namespace gogort {

class DynamicScheduler : public Scheduler {
private:
  std::vector<std::shared_ptr<Worker>> &workers_;
  std::list<std::shared_ptr<Routine>> normal_routines_;
  std::list<std::shared_ptr<Routine>> quick_reactive_routines_;

  std::vector<std::shared_ptr<RiskInstance>> pending_instances_;
  std::vector<std::shared_ptr<RiskInstance>> handled_instances_;
  std::vector<std::shared_ptr<Risk>> registered_risks_;

private:
  bool DoOnce() override;

public:
  explicit DynamicScheduler(std::vector<std::shared_ptr<Worker>> &);
  bool AddRoutine(std::shared_ptr<Routine> routine) override;
};

} // namespace gogort

#endif // GOGO_PROJ_DYNAMICSCHEDULER_H
