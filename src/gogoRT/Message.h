//
// Created by Yuting Xie on 03/12/2022.

#ifndef GOGO_MESSAGE_H
#define GOGO_MESSAGE_H

#include "utils/utils.h"
#include <string>

#define M_SIZE (1024)

namespace gogort {

class Message {
private:
  uint64 timestamp_;

public:
  Message() : timestamp_(get_next_uuid()) {}
  // Message should do hard-copied.
  Message &operator=(Message &&other_msg) noexcept {
    timestamp_ = other_msg.timestamp_;
    return *this;
  }
  virtual ~Message() = default;
  ;
  virtual std::string to_string() = 0;
  uint64 get_timestamp() const { return timestamp_; }
};

} // namespace gogort

#endif // GOGO_MESSAGE_H
