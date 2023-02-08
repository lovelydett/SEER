//
// Created by Yuting Xie on 08/02/2023.
//

#include "DummyRisk1Detector.h"

namespace gogort {

DummyRisk1Detector::DummyRisk1Detector(std::shared_ptr<Pipe> sensor_pipe)
    : reader_(sensor_pipe) {}

std::shared_ptr<Risk> DummyRisk1Detector::anticipate_once() {
  auto risk_msg = std::make_shared<message::DummyRisk1>(0.f, 0.f);
  auto msg = reader_.Read();
  if (msg != nullptr) {
    // Todo(yuting): supposed to do some anticipation here
    return std::dynamic_pointer_cast<Risk>(risk_msg);
  }
  return nullptr;
}

} // namespace gogort