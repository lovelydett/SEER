//
// Created by Yuting Xie on 03/12/2022.
//

#include "DummyMessage.h"
#include "../gogoRT/utils/utils.h"

#include <cstdlib>

namespace message {

DummyMessage::DummyMessage() {
  matrix = (double(*)[M_SIZE])malloc(sizeof(double) * M_SIZE * M_SIZE);
}

std::string DummyMessage::to_string() { return "This is a DummyMessage"; }

DummyMessage::~DummyMessage() { SAFE_FREE(matrix); }

} // namespace message