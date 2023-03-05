//
// Created by Yuting Xie on 08/02/2023.
//

#include "DummyRisk1.h"
#include "../utils/utils.h"

#include <functional>

namespace gogort {
DummyRisk1Instance::DummyRisk1Instance(float zeta, float kappa)
    : RiskInstance(zeta, kappa) {
  // TODO(yuting): Acquire the pipe name from the config file
  pipe_to_match_ = AcquireReader<message::DummyMessage>("pipe_risk1");
}

bool DummyRisk1Instance::Match() {
  auto &&msg = pipe_to_match_->Read();
  if (msg == nullptr) {
    return false;
  }

  // Todo(yuting): Implement the matching logic

  return false;
}
std::shared_ptr<Routine> DummyRisk1Instance::GetHandler() {
  if (reactive_control_ != nullptr) { // Already handled
    return nullptr;
  }
  // Return a routine that pends execution to get a control command.
  return std::make_shared<Routine>(
      std::bind(&DummyRisk1Instance::inner_handler, this), "risk1_handler");
}

std::shared_ptr<message::ControlCommand>
DummyRisk1Instance::GetReactiveControl() {
  return reactive_control_;
}

bool DummyRisk1Instance::IsExpired() { return false; }

std::list<std::shared_ptr<gogort::RiskInstance>> DummyRisk1::Detect() {
  std::list<std::shared_ptr<gogort::RiskInstance>> detected_risks;
  auto &&msg = pipe_to_detect_->Read();
  if (msg == nullptr) {
    return detected_risks;
  }
  // Todo(yuting): Implement the detection logic
  return detected_risks;
}
} // namespace gogort