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

#define M_SIZE (1024)

class DummyTask3 : public Task<DummyMessage, DummyMessage2> {
private:
  float vec[M_SIZE];

private:
  int count;
  bool init_config(std::string) override;

public:
  DummyTask3() = delete;
  explicit DummyTask3(std::string);
  bool Deal(const std::shared_ptr<DummyMessage>,
            const std::shared_ptr<DummyMessage2>) override;
  std::shared_ptr<gogort::InvokerBase> get_invoker() override;
};

} // namespace task

#endif // GOGO_PROJ_DUMMYTASK3_H
