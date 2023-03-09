//
// Created by Yuting Xie on 20/02/2023.
//

#include "RiskFactory.h"
#include "risk_type/DummyRisk1.h"

namespace gogort {
RiskFactory *RiskFactory::Instance() { return &instance_; }
std::shared_ptr<Risk> RiskFactory::CreateRisk(const std::string risk_name) {
  if (risk_name == "DummyRisk1") {
    return std::make_shared<DummyRisk1>();
  }
  return nullptr;
}
} // namespace gogort