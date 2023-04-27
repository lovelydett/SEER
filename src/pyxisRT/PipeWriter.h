//
// Created by Yuting Xie
// 23-4-28

#ifndef SEER_PIPEWRITER_H
#define SEER_PIPEWRITER_H

#include "Pipe.h"

#include <memory>

namespace pyxis {

template <typename MSG> class PipeWriter {
private:
  Pipe pipe_;

public:
  PipeWriter(const Pipe &pipe) : pipe_(pipe) {}
  ~PipeWriter() = default;
  bool Write(const std::shared_ptr<MSG> msg) { return pipe_.Write(msg.get()); }
};

} // namespace pyxis

#endif // SEER_PIPEWRITER_H
