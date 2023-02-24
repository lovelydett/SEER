//
// Created by Yuting Xie on 18/02/2023.
//

#include "DynamicScheduler.h"

namespace gogort {

DynamicScheduler::DynamicScheduler(
    std::vector<std::shared_ptr<Worker>> &workers)
    : workers_(workers) {
  risk_manager_ = RiskManager::Instance();
}

bool DynamicScheduler::DoOnce() {
  // 0. Do normal scheduling, only react risks when there are idle workers.
  // Todo(yuting): use other scheduling plugins here.

  // 1. Check if there are idle workers.
  bool any_idle = false;
  for (auto &worker : workers_) {
    if (!worker->isBusy()) {
      any_idle = true;
      break;
    }
  }
  if (!any_idle) {
    return true;
  }

  // There are idle workers, handle risks.
  // 1. Check if interested sensory inputs are updated, if not, do nothing.
  if (!risk_manager_->check_sensory_updates()) {
    return true;
  }

  // 2. Check if handled risks are matched, if any, apply quick reactive control
  auto &&matched_risks = risk_manager_->match_sensory_updates();
  if (!matched_risks.empty()) {
    // Risks matched, applied the quick reactive control command.
    // Todo(yuting): pick a matched risk by some policy.
    gogo_id_t picked_risk_id = matched_risks[0]->get_id();
    auto control_command = risk_manager_->get_control_command(picked_risk_id);
    // Todo(yuting): apply the control command.
  }

  // 3. Detect new risks from new sensory inputs.
  auto &&QRRs = risk_manager_->new_quick_reactive_routines();
  // Todo(yuting): rearrange the newly detected QRRs by some policy.
  if (!QRRs.empty()) {
    // New risks detected, add them to the quick reactive routine list.
    quick_reactive_routines_.insert(quick_reactive_routines_.end(),
                                    QRRs.begin(), QRRs.end());
  }

  // 4. Schedule the quick reactive routines on idle workers.

  return true;
}

} // namespace gogort