//
// Created by Yuting Xie on 03/12/2022.

#ifndef GOGO_MESSAGE_H
#define GOGO_MESSAGE_H

#include "utils/utils.h"
#include <string>

#define M_SIZE (2048)

namespace gogort {

class Message {
private:
  uint64 timestamp_;

public:
  Message() : timestamp_(get_next_uuid()) { ; }
  // Message should do hard-copied.
  virtual Message &operator=(const Message &&other_msg) {
    timestamp_ = other_msg.timestamp_;
    return *this;
  }
  virtual ~Message(){};
  virtual std::string to_string() = 0;
  uint64 get_timestamp() { return timestamp_; }
};

} // namespace gogort

#endif // GOGO_MESSAGE_H
