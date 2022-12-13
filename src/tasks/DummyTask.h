//
// Created by Yuting Xie on 03/12/2022.
//

#ifndef GOGO_DUMMYTASK_H
#define GOGO_DUMMYTASK_H

#include "../gogoRT/Invoker.h"
#include "../messages/DummyMessage.h"
#include "Task.h"
#include <memory>

using gogort::Task;
using message::DummyMessage;

namespace task {

class DummyTask : public Task<DummyMessage> {
private:
  double vec[M_SIZE];
  bool init_config(const std::string) override;

public:
  DummyTask() : Task<DummyMessage>("DummyTask"){};
  bool Deal(const std::shared_ptr<DummyMessage> &) override;
  std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

} // namespace task

#endif // GOGO_DUMMYTASK_H
