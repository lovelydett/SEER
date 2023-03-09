//
// Created by Yuting Xie on 08/02/2023.
//

#ifndef GOGO_PROJ_DUMMYRISK1_H
#define GOGO_PROJ_DUMMYRISK1_H

#include "../../Comm/CommBuffer.h"
#include "../../Routine.h"
#include "../../messages/ControlCommand.h"
#include "../../messages/DummyMessage.h"
#include "../Risk.h"
#include "../RiskInstance.h"

#include <chrono>
#include <memory>

namespace gogort {

typedef struct Point2D {
  float x = 0.;
  float y = 0.;
} Point2D;

class DummyRisk1Instance : public RiskInstance {
private:
  Point2D location_;
  float range_;
  std::shared_ptr<PipeReader<message::DummyMessage>> pipe_to_match_;
  std::shared_ptr<message::ControlCommand> reactive_control_;

private:
  void inner_handler();

public:
  DummyRisk1Instance(float zeta, float kappa);
  bool Match() override;
  std::shared_ptr<Routine> GetHandler() override;
  std::shared_ptr<message::ControlCommand> GetReactiveControl() override;
  bool IsExpired() override;
  std::string get_risk_name() override;
};

class DummyRisk1 : public gogort::Risk {
private:
  std::shared_ptr<PipeReader<message::DummyMessage>> pipe_to_detect_;
  double interval_ms_;
  std::chrono::system_clock::time_point time_point_;

public:
  DummyRisk1();
  std::list<std::shared_ptr<gogort::RiskInstance>> Detect() override;
};

} // namespace gogort

#endif // GOGO_PROJ_DUMMYRISK1_H
