//
// Created by Yuting Xie on 20/02/2023.
//

#ifndef GOGO_PROJ_RISKMANAGER_H
#define GOGO_PROJ_RISKMANAGER_H

#include "../../messages/ControlCommand.h"
#include "../Routine.h"
#include "Risk.h"
#include "RiskFactory.h"

#include <map>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <vector>

namespace gogort {

using message::ControlCommand;

/**
 * @class RiskManager RiskManage.h RiskManager.cpp
 * @brief It manages the real-time risks that pends handling and already handled
 */
class RiskManager {
private:
  RiskManager() = default;
  bool init_config();

private:
  static std::mutex instance_lock_;
  static RiskManager *instance_;
  std::unordered_map<gogo_id_t, std::shared_ptr<Risk>> id_to_handled_risks_;
  std::unordered_map<gogo_id_t, std::shared_ptr<Risk>> id_to_pending_risks_;
  std::unordered_map<gogo_id_t, std::shared_ptr<ControlCommand>>
      id_to_quick_reactive_control_;
  std::priority_queue<std::shared_ptr<Message>> sensory_updates_;

public:
  static RiskManager *Instance();

  // This class provides APIs for risk handling:
  // 1. Check new sensory inputs.
  bool check_sensory_updates();

  // 2. (Match) Match new sensory inputs to existing risks, if any.
  std::vector<std::shared_ptr<Risk>> match_sensory_updates();

  // 3. Get corresponding control command for a specific handled risk.
  std::shared_ptr<ControlCommand> get_control_command(gogo_id_t risk_id);

  // 4. (Detect) Detect new risks from new sensory inputs.
  // return all NEW QRTs
  std::vector<std::shared_ptr<Routine>> new_quick_reactive_routines();
};

} // namespace gogort

#endif // GOGO_PROJ_RISKMANAGER_H
