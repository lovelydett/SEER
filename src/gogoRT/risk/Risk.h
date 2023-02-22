//
// Created by Yuting Xie on 22/02/2023.
//

#ifndef GOGO_PROJ_RISK_H
#define GOGO_PROJ_RISK_H

#include "../utils/utils.h"

namespace gogort {

class Risk {
protected:
  float zeta_;  // probability
  float kappa_; // potential damage
  gogo_id_t id_;

public:
  Risk() = delete;
  Risk(float zeta, float kappa);
  void set_zeta(float zeta);
  void set_kappa(const float kappa);
  [[nodiscard]] float get_zeta() const;
  [[nodiscard]] float get_kappa() const;
  [[nodiscard]] gogo_id_t get_id() const;
  virtual std::string get_risk_name() = 0;
};

} // namespace gogort

#endif // GOGO_PROJ_RISK_H
