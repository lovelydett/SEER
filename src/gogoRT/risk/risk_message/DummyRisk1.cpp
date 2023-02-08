//
// Created by Yuting Xie on 08/02/2023.
//

#include "DummyRisk1.h"

namespace message {

DummyRisk1::DummyRisk1(float zeta, float kappa) : Risk(zeta, kappa) {}

std::string DummyRisk1::to_string() { return "This is a DummyRisk1 message"; }

std::string DummyRisk1::get_risk_name() {
  return "This is a DummyRisk1-type risk";
}

} // namespace message