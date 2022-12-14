//
// Created by Yuting Xie on 03/12/2022.
//

#ifndef GOGO_DUMMYMESSAGE_H
#define GOGO_DUMMYMESSAGE_H

#include "../gogoRT/Message.h"

namespace message {

#define M_SIZE (1024)

class DummyMessage : public gogort::Message {
public:
  double (*matrix)[M_SIZE];

public:
  DummyMessage();
  ~DummyMessage() override;
  std::string to_string() override;
};

} // namespace message

#endif // GOGO_DUMMYMESSAGE_H
