// For active pipe reading, not in use now
// Created by Yuting Xie
// 22-12-6

#ifndef GOGO_PIPEREADER_H
#define GOGO_PIPEREADER_H

#include "../utils/utils.h"
#include "Pipe.h"
#include <memory>
#include <optional>

namespace gogort {

template <class MSG> class PipeReader {
private:
  std::weak_ptr<Pipe *const> pipe_;

  // Yuting@2022.12.6: the status of readability should be maintained by a
  // reader rather a pipe, since readers are task-owned, a piece of message may
  // expire for some tasks while still alive for the others not yet consume it.
  time_t ts_updated_;

public:
  PipeReader() = delete;
  PipeReader(const PipeReader &) = delete;
  PipeReader &operator=(const PipeReader &) = delete;
  PipeReader(PipeReader &&) = delete;
  PipeReader(const std::shared_ptr<Pipe *const> &pipe) : pipe_(pipe) {}
  virtual ~PipeReader() = default;

  bool isUpdated() const {
    auto valid_pipe = pipe_.lock();
    return ts_updated_ < valid_pipe->get_timestamp();
  }
};

} // namespace gogort

#endif // GOGO_PIPEREADER_H
