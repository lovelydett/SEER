//
// Created by Yuting Xie on 03/12/2022.
//

#ifndef GOGO_DUMMYTASK_H
#define GOGO_DUMMYTASK_H

#include "../gogoRT/Comm/PipeWriter.h"
#include "../gogoRT/Invoker.h"
#include "../messages/DummyMessage.h"
#include "Task.h"

#include <memory>

using gogort::Task;
using message::DummyMessage;

namespace task {

class DummyTask : public Task<> {
private:
  int count;
  bool init_config(std::string) override;
  std::shared_ptr<gogort::PipeWriter<DummyMessage>> writer_;

public:
  DummyTask() = delete;
  DummyTask(std::string, std::string);
  bool Deal() override;
  std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

} // namespace task

#endif // GOGO_DUMMYTASK_H
