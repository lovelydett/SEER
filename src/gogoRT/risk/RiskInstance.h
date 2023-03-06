//
// Created by Yuting Xie on 05/03/2023.
//

#ifndef GOGO_PROJ_RISKINSTANCE_H
#define GOGO_PROJ_RISKINSTANCE_H

#include "../../messages/ControlCommand.h"
#include "../Routine.h"

#include <memory>

namespace gogort {

class RiskInstance {
protected:
  float zeta_;  // probability
  float kappa_; // potential damage
  gogo_id_t id_;
  uint64 timestamp_;

public:
  RiskInstance() = delete;
  RiskInstance(float zeta, float kappa)
      : zeta_(zeta), kappa_(kappa), id_(get_next_uuid()),
        timestamp_(get_current_timestamp_ms()) {}
  virtual ~RiskInstance() = default;
  virtual bool Match() = 0;
  virtual std::shared_ptr<Routine> GetHandler() = 0;
  virtual std::shared_ptr<message::ControlCommand> GetReactiveControl() = 0;
  virtual bool IsExpired() = 0;
  virtual bool IsMoreCriticalThan(const std::shared_ptr<RiskInstance> other) {
    return zeta_ * kappa_ > other->get_zeta() * other->get_kappa();
  }

  void set_zeta(float zeta) { zeta_ = zeta; }
  void set_kappa(const float kappa) { kappa_ = kappa; }
  [[nodiscard]] float get_zeta() const { return zeta_; }
  [[nodiscard]] float get_kappa() const { return kappa_; }
  [[nodiscard]] gogo_id_t get_id() const { return id_; }
  [[nodiscard]] uint64 get_timestamp() const { return timestamp_; }
  virtual std::string get_risk_name() = 0;
};

} // namespace gogort

#endif // GOGO_PROJ_RISKINSTANCE_H
