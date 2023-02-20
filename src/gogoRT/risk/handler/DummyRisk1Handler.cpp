//
// Created by Yuting Xie on 18/02/2023.
//

#include "DummyRisk1Handler.h"

namespace gogort {

bool DummyRisk1Handler::Deal(std::shared_ptr<DummyRisk1> msg) {
  // Compute a reactive control message, output to a designated pipe.
  return true;
}

} // namespace gogort