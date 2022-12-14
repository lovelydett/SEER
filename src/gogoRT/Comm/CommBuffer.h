//
// Created by Yuting Xie
// 22-9-13

#ifndef GOGO_COMMBUFFER_H
#define GOGO_COMMBUFFER_H

#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>

#include "Pipe.h"
#include "PipeReader.h"
#include "PipeWriter.h"

namespace gogort {

class CommBuffer {
private:
  bool is_init_ = false;
  static std::shared_ptr<CommBuffer> instance_;
  static std::mutex mtx_;

private:
  CommBuffer();
  bool init_config(const std::string config_path);
  // Pipe name to pipe
  std::unordered_map<std::string, std::shared_ptr<Pipe>> name_to_pipe_;

public:
  static std::shared_ptr<CommBuffer> Instance();
  std::shared_ptr<Pipe> GetPipe(const std::string pipe_name);
};

// Tell CommBuffer you need a reader
template <class MSG>
std::shared_ptr<PipeReader<MSG>> AcquireReader(const std::string pipe_name) {
  auto pipe = CommBuffer::Instance()->GetPipe(pipe_name);
  if (pipe) {
    return std::make_shared<PipeReader<MSG>>(pipe);
  }
  return nullptr;
}

// Get the writer of a pipe.
template <class MSG>
std::shared_ptr<PipeWriter<MSG>> AcquireWriter(const std::string pipe_name) {
  auto pipe = CommBuffer::Instance()->GetPipe(pipe_name);
  if (pipe) {
    return std::make_shared<PipeWriter<MSG>>(pipe);
  }
  return nullptr;
}

} // namespace gogort

#endif // GOGO_COMMBUFFER_H
