// For active pipe reading, not in use now
// Created by Yuting Xie
// 22-12-6

#ifndef GOGO_PIPEREADER_H
#define GOGO_PIPEREADER_H

#include "Pipe.h"
#include <memory>

namespace gogort {

template <class MSG> class PipeReader {
private:
  std::weak_ptr<Pipe *const> pipe_;

public:
  PipeReader() = delete;
  PipeReader(const PipeReader &) = delete;
  PipeReader &operator=(const PipeReader &) = delete;
  PipeReader(PipeReader &&) = delete;
  PipeReader(const std::shared_ptr<Pipe *const> &pipe) : pipe_(pipe) {}
  virtual ~PipeReader() = default;
};

} // namespace gogort

#endif // GOGO_PIPEREADER_H
