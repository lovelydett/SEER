//
// Created by Yuting Xie on 27/12/2022.
//

#include "DummyMessage2.h"

namespace message {

DummyMessage2::DummyMessage2() {
  bias = (double *)malloc(sizeof(double) * M_SIZE);
}

std::string DummyMessage2::to_string() { return "This is a DummyMessage2"; }

DummyMessage2::~DummyMessage2() { SAFE_FREE(bias); }

} // namespace message