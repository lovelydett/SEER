//
// Created by Yuting Xie on 03/12/2022.

#ifndef GOGO_MESSAGE_H
#define GOGO_MESSAGE_H

#include <string>

namespace gogort {

class Message {
public:
  Message() = delete;
  virtual ~Message() = 0;
  virtual std::string to_string() = 0;
};

} // namespace gogort

#endif // GOGO_MESSAGE_H
