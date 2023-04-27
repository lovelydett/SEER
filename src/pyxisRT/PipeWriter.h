//
// Created by Yuting Xie
// 23-4-28

#ifndef GOGO_PROJ_PIPEWRITER_H
#define GOGO_PROJ_PIPEWRITER_H

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

#endif // GOGO_PROJ_PIPEWRITER_H
