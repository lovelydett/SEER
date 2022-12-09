// For active pipe writing
// Created by Yuting Xie
// 22-12-6

#ifndef GOGO_PIPEWRITER_H
#define GOGO_PIPEWRITER_H

#include "Pipe.h"
#include <memory>

namespace gogort {

template <class MSG> class PipeWriter {
private:
  std::weak_ptr<Pipe> pipe_;

public:
  PipeWriter() = delete;
  PipeWriter(const PipeWriter &) = delete;
  PipeWriter &operator=(const PipeWriter &) = delete;
  PipeWriter(PipeWriter &&) = delete;
  explicit PipeWriter(const std::shared_ptr<Pipe> pipe) : pipe_(pipe) {}
  virtual ~PipeWriter() = default;

  bool Publish(MSG &&message) {
    // Todo(yuting): publish message to this pipe
    auto valid_pipe = pipe_.lock();
    return valid_pipe->Enqueue(message);
  }
};

} // namespace gogort

#endif // GOGO_PIPEWRITER_H
