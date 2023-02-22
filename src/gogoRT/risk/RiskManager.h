//
// Created by Yuting Xie on 20/02/2023.
//

#ifndef GOGO_PROJ_RISKMANAGER_H
#define GOGO_PROJ_RISKMANAGER_H

#include "Risk.h"
#include "RiskFactory.h"

#include <mutex>
#include <unordered_map>

namespace gogort {

/**
 * @class RiskManager RiskManage.h RiskManager.cpp
 * @brief It manages the real-time risks that pends handling and already handled
 */
class RiskManager {
private:
  RiskManager() = default;
  static std::mutex instance_lock_;
  static RiskManager *instance_;
  std::unordered_map<gogo_id_t, >()

public:
  static RiskManager *Instance();
};

} // namespace gogort

#endif // GOGO_PROJ_RISKMANAGER_H
