// For active pipe reading, not in use now
// Created by Yuting Xie
// 22-12-6

#ifndef GOGO_PIPEREADER_H
#define GOGO_PIPEREADER_H

#include "../utils/utils.h"
#include "Pipe.h"
#include <cassert>
#include <glog/logging.h>
#include <memory>
#include <optional>

namespace gogort {

template <class MSG> class PipeReader {
private:
  std::shared_ptr<Pipe> pipe_;

  // Yuting@2022.12.6: the status of readability should be maintained by a
  // reader rather a pipe, since readers are task-owned, a piece of message may
  // expire for some tasks while still alive for the others not yet consume it.
  time_t ts_updated_{};
  bool isUpdated() const {
    assert(pipe_ != nullptr);
    return ts_updated_ < pipe_->get_timestamp();
  }

public:
  PipeReader() = delete;
  PipeReader(const PipeReader &) = delete;
  PipeReader &operator=(const PipeReader &) = delete;
  PipeReader(PipeReader &&) = delete;
  explicit PipeReader(const std::shared_ptr<Pipe> pipe) : pipe_(pipe) {
    assert(pipe_ != nullptr);
  }
  virtual ~PipeReader() = default;

  std::shared_ptr<MSG> Read() {
    if (isUpdated()) {
      std::shared_ptr<MSG> msg = std::dynamic_pointer_cast<MSG>(pipe_->Top());
      // Todo(yuting): Messages should implement copy constructor.
      return msg;
    }
    return nullptr;
  }
};

} // namespace gogort

#endif // GOGO_PIPEREADER_H
