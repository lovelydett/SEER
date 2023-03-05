//
// Created by Yuting Xie on 20/02/2023.
//

#ifndef GOGO_PROJ_RISKFACTORY_H
#define GOGO_PROJ_RISKFACTORY_H

#include "Risk.h"

#include <string>

namespace gogort {

class RiskFactory {
private:
  static RiskFactory instance_;

public:
  static RiskFactory *Instance();
  std::shared_ptr<Risk> CreateRisk(std::string risk_name);
};

} // namespace gogort

#endif // GOGO_PROJ_RISKFACTORY_H
