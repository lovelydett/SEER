//
// Created by Yuting Xie on 27/12/2022.
//

#ifndef GOGO_PROJ_DUMMYTASK2_H
#define GOGO_PROJ_DUMMYTASK2_H

#include "../gogoRT/Comm/PipeWriter.h"
#include "../gogoRT/Invoker.h"
#include "../messages/DummyMessage2.h"
#include "Task.h"

using gogort::Task;
using message::DummyMessage2;

namespace task {

class DummyTask2 : public Task<> {
private:
  int count;
  bool init_config(std::string) override;
  std::shared_ptr<gogort::PipeWriter<DummyMessage2>> writer_;

public:
  DummyTask2() = delete;
  DummyTask2(std::string task_name, std::string config_file,
             std::vector<std::string> &&in_pipes,
             std::vector<std::string> &&out_pipes);
  bool Deal() override;
  [[nodiscard]] std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

} // namespace task

#endif // GOGO_PROJ_DUMMYTASK2_H
