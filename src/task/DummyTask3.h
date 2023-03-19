//
// Created by Yuting Xie on 27/12/2022.
//

#ifndef GOGO_PROJ_DUMMYTASK3_H
#define GOGO_PROJ_DUMMYTASK3_H

#include "../gogoRT/Task.h"
#include "../messages/DummyMessage.h"
#include "../messages/DummyMessage2.h"

using gogort::Task;
using message::DummyMessage;
using message::DummyMessage2;

namespace task {

class DummyTask3 : public Task<DummyMessage, DummyMessage2> {
private:
  float vec[M_SIZE];

private:
  int count;
  bool init_config(std::string) override;

public:
  DummyTask3() = delete;
  DummyTask3(std::string task_name, std::string config_file,
             std::vector<std::string> &&in_pipes,
             std::vector<std::string> &&out_pipes);
  bool Deal(std::shared_ptr<DummyMessage>,
            std::shared_ptr<DummyMessage2>) override;
  std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

} // namespace task

#endif // GOGO_PROJ_DUMMYTASK3_H
