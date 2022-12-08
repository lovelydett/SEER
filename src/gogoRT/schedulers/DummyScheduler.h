//
// Created by Yuting Xie
// 2022/12/8
#ifndef GOGORT_DUMMYSCHEDULER_H
#define GOGORT_DUMMYSCHEDULER_H

#include "../Scheduler.h"

namespace gogort {

class DummyScheduler : public Scheduler {
public:
  explicit DummyScheduler(std::vector<std::shared_ptr<Worker>>&);
  bool DoOnce() override;
};

} // namespace gogort

#endif // GOGORT_DUMMYSCHEDULER_H
