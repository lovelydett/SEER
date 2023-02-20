//
// Created by Yuting Xie on 18/02/2023.
//

#ifndef GOGO_PROJ_DUMMYRISK1HANDLER_H
#define GOGO_PROJ_DUMMYRISK1HANDLER_H

#include "../../Task.h"
#include "../risk_message/DummyRisk1.h"

#include <memory>

namespace gogort {

using message::DummyRisk1;

class DummyRisk1Handler : public Task<DummyRisk1> {
public:
  bool Deal(std::shared_ptr<DummyRisk1> msg);
};

} // namespace gogort

#endif // GOGO_PROJ_DUMMYRISK1HANDLER_H
