//
// Created by Yuting Xie on 08/02/2023.
//

#ifndef GOGO_PROJ_RISK_H
#define GOGO_PROJ_RISK_H

#include <string>

namespace gogort {

class Risk {
protected:
  float zeta_;  // probability
  float kappa_; // potential damage

public:
  Risk() = delete;
  Risk(const float zeta, const float kappa);
  void set_zeta(const float zeta);
  void set_kappa(const float kappa);
  float get_zeta() const;
  float get_kappa() const;
  virtual std::string get_risk_name() = 0;
};

} // namespace gogort

#endif // GOGO_PROJ_RISK_H
