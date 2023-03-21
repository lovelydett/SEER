//
// Created by Yuting Xie on 08/02/2023.
//

#ifndef GOGO_PROJ_DETECTOR_H
#define GOGO_PROJ_DETECTOR_H

#include "../Message.h"
#include "Risk.h"

#include <memory>

namespace gogort {

class Detector {
public:
  Detector() = default;
  virtual std::shared_ptr<Risk> anticipate_once() = 0;
};

} // namespace gogort

#endif // GOGO_PROJ_DETECTOR_H
