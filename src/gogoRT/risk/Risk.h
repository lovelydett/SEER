//
// Created by Yuting Xie on 22/02/2023.
//

#ifndef GOGO_PROJ_RISK_H
#define GOGO_PROJ_RISK_H

#include "../utils/utils.h"
#include "RiskInstance.h"

#include <list>
#include <memory>

namespace gogort {

class Risk {
public:
  virtual std::list<std::shared_ptr<RiskInstance>> Detect() = 0;
};

} // namespace gogort

#endif // GOGO_PROJ_RISK_H
