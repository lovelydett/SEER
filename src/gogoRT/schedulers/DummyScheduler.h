//
// Created by Yuting Xie
// 2022/12/8
#ifndef GOGORT_DUMMYSCHEDULER_H
#define GOGORT_DUMMYSCHEDULER_H

#include "../Scheduler.h"

namespace gogort {

class DummyScheduler : public Scheduler {
private:
  std::vector<std::shared_ptr<Worker>> &workers_;
  std::list<std::shared_ptr<Routine>> routines_;

private:
  bool DoOnce() override;

public:
  explicit DummyScheduler(std::vector<std::shared_ptr<Worker>> &);
  bool AddRoutine(std::shared_ptr<Routine> routine) override;
};

} // namespace gogort

#endif // GOGORT_DUMMYSCHEDULER_H
