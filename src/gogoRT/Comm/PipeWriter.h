// For active pipe writing
// Created by Yuting Xie
// 22-12-6

#ifndef GOGO_PIPEWRITER_H
#define GOGO_PIPEWRITER_H

#include <string>

template <class MSG>
class PipeWriter {
private:
  std::string pipe_name_;

public:
  PipeWriter() = delete;
  PipeWriter(const PipeWriter &) = delete;
  PipeWriter &operator=(const PipeWriter &) = delete;
  PipeWriter(PipeWriter &&) = delete;
  PipeWriter(std::string pipe_name) : pipe_name_(pipe_name) {}
  virtual ~PipeWriter() = default;

  bool Publish(const MSG&& message) {
    // Todo(yuting): publish message to this pipe
    return true;
  }

};

#endif // GOGO_PIPEWRITER_H
