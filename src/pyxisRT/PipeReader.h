//
// Created by Yuting Xie
// 23-4-28

#ifndef SEER_PIPEREADER_H
#define SEER_PIPEREADER_H

#include "Pipe.h"

#include <memory>

namespace pyxis {

template <typename MSG> class PipeReader {
private:
  Pipe pipe_;
  uint64_t last_timestamp_ms_;

public:
  PipeReader(const Pipe &pipe) : pipe_(pipe), last_timestamp_ms_(0) {}
  ~PipeReader() = default;
  std::shared_ptr<MSG> Read() {
    std::shared_ptr<MSG> msg = std::make_shared<MSG>();
    // If no newer message, last_timestamp wont be modified.
    if (pipe_.Read(msg.get(), &last_timestamp_ms_) == false) {
      msg = nullptr;
    }
    return msg;
  }
};

} // namespace pyxis

#endif // SEER_PIPEREADER_H
