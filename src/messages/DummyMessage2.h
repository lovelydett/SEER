//
// Created by Yuting Xie on 27/12/2022.
//

#ifndef GOGO_PROJ_DUMMYMESSAGE2_H
#define GOGO_PROJ_DUMMYMESSAGE2_H

#include "../gogoRT/Message.h"

namespace message {

#define M_SIZE (1024)

class DummyMessage2 : public gogort::Message {
public:
  double *bias;

public:
  DummyMessage2();
  ~DummyMessage2() override;
  std::string to_string() override;
};

} // namespace message

#endif // GOGO_PROJ_DUMMYMESSAGE2_H
