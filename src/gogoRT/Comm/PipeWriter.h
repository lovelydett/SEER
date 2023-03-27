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
  std::shared_ptr<Pipe> pipe_;

public:
  PipeWriter() = delete;
  PipeWriter(const PipeWriter &) = delete;
  PipeWriter &operator=(const PipeWriter &) = delete;
  PipeWriter(PipeWriter &&) = delete;
  explicit PipeWriter(const std::shared_ptr<Pipe> pipe) : pipe_(pipe) {}
  virtual ~PipeWriter() = default;

  bool Publish(std::shared_ptr<MSG> message) {
    auto message_base = std::dynamic_pointer_cast<Message>(message);
    assert(message_base != nullptr);
    return pipe_->Enqueue(message_base);
  }
};

} // namespace gogort

#endif // GOGO_PIPEWRITER_H
