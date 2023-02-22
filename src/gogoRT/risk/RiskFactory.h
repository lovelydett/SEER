//
// Created by Yuting Xie on 20/02/2023.
//

#ifndef GOGO_PROJ_RISKFACTORY_H
#define GOGO_PROJ_RISKFACTORY_H

#include "../Task.h"
#include "Risk.h"

#include <string>

namespace gogort {

class RiskFactory {
private:
  static RiskFactory *instance_;

public:
  static RiskFactory *Instance();
  std::shared_ptr<TaskBase> CreateRisk(std::string risk_name);
};

} // namespace gogort

#endif // GOGO_PROJ_RISKFACTORY_H
