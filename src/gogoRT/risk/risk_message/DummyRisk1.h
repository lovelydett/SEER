//
// Created by Yuting Xie on 08/02/2023.
//

#ifndef GOGO_PROJ_DUMMYRISK1_H
#define GOGO_PROJ_DUMMYRISK1_H

#include "../../Message.h"
#include "../Risk.h"

namespace message {

typedef struct Point2D {
  float x = 0.;
  float y = 0.;
} Point2D;

class DummyRisk1 : public gogort::Message, public gogort::Risk {
private:
  Point2D location_;
  float range_;

public:
  DummyRisk1(float zeta, float kappa);
  std::string to_string() override;
  std::string get_risk_name() override;
};

} // namespace message

#endif // GOGO_PROJ_DUMMYRISK1_H
