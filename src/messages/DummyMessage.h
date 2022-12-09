//
// Created by Yuting Xie on 03/12/2022.
//

#ifndef GOGO_DUMMYMESSAGE_H
#define GOGO_DUMMYMESSAGE_H

namespace message {

#define M_SIZE (1024)

class DummyMessage {
public:
  double matrix[M_SIZE][M_SIZE];
};

} // namespace message

#endif // GOGO_DUMMYMESSAGE_H
