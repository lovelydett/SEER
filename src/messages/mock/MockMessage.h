//
// Created by Yuting Xie on 16/03/2023.
//

#ifndef GOGO_PROJ_MOCKMESSAGE_H
#define GOGO_PROJ_MOCKMESSAGE_H

#include "../../Message.h"

#include <string>

using gogort::Message;

namespace message {

class MockMessage : public Message {
public:
  std::string to_string();
};

} // namespace message

#endif // GOGO_PROJ_MOCKMESSAGE_H
