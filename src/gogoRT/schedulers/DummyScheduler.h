//
// Created by Yuting Xie
// 2022/12/8
#ifndef GOGORT_DUMMYSCHEDULER_H
#define GOGORT_DUMMYSCHEDULER_H

#include "../Scheduler.h"

#include <unordered_map>

namespace gogort {

class DummyScheduler : public Scheduler {
private:
  std::list<std::shared_ptr<Routine>> routines_;
  // Yuting@2023.3.23: we must consider the task graph cycle order now
  std::unordered_map<std::string, uint64_t> latest_tasks_;

private:
  bool DoOnce() override;

public:
  explicit DummyScheduler(std::vector<std::shared_ptr<Worker>> &);
  bool AddRoutine(std::shared_ptr<Routine> routine) override;
};

} // namespace gogort

#endif // GOGORT_DUMMYSCHEDULER_H
