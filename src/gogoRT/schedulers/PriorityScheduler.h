//
// Created by Yuting Xie on 09/01/2023.
//

#ifndef GOGO_PROJ_PRIORITYSCHEDULER_H
#define GOGO_PROJ_PRIORITYSCHEDULER_H

#include "../Dispatcher.h"
#include "../Scheduler.h"

#include <memory>
#include <queue>
#include <unordered_map>

namespace gogort {

typedef std::vector<std::shared_ptr<Worker>> WorkerVec;
typedef std::shared_ptr<Routine> RoutinePtr;
typedef std::vector<RoutinePtr> RoutineVec;

class RoutineCmp {
public:
  bool operator()(const RoutinePtr &routine1, const RoutinePtr &routine2);
};

class PriorityScheduler : public Scheduler {
private:
  std::priority_queue<RoutinePtr, RoutineVec, RoutineCmp> routines_;
  // Yuting@2023-03-26: For now we just dont consider OS thread priority
  // std::unordered_map<uint16, WorkerVec> workers_;
  WorkerVec workers_;

private:
  bool DoOnce() override;

public:
  explicit PriorityScheduler(std::vector<std::shared_ptr<Worker>> &);
  bool AddRoutine(std::shared_ptr<Routine> routine) override;
};

} // namespace gogort

#endif // GOGO_PROJ_PRIORITYSCHEDULER_H
