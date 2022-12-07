//
// Created by Yuting Xie on 03/12/2022.
//

#ifndef GOGO_DUMMYTASK_H
#define GOGO_DUMMYTASK_H

#include "DummyMessage.h"
#include "Task.h"
using gogort::Task;
using message::DummyMessage;

namespace task {

class DummyTask : public Task<DummyMessage> {
private:
  bool init_config(const std::string) override;

public:
  DummyTask() : Task("DummyTask"){};
  bool Deal(const std::shared_ptr<DummyMessage> &) override;
};

} // namespace task

#endif // GOGO_DUMMYTASK_H
