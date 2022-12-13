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

public:
  static std::shared_ptr<CommBuffer> Instance();
  // Pipe name to pipe
  std::unordered_map<std::string, std::shared_ptr<Pipe>> name_to_pipe_;
};

// Tell CommBuffer you need a reader
template <class MSG>
std::shared_ptr<PipeReader<MSG>> AcquireReader(const std::string pipe_name) {
  auto &name_to_pipe = CommBuffer::Instance()->name_to_pipe_;
  auto it = name_to_pipe.find(pipe_name);
  if (name_to_pipe.end() != it) {
    return nullptr;
  }
  // This pipe exists, use it to initialize a reader.
  auto pipe = it->second;
  return std::make_shared<PipeReader<MSG>>(pipe);
}

// Get the writer of a pipe.
template <class MSG>
std::shared_ptr<PipeReader<MSG>> AcquireWriter(const std::string pipe_name) {
  auto &name_to_pipe = CommBuffer::Instance()->name_to_pipe_;
  auto it = name_to_pipe.find(pipe_name);
  if (name_to_pipe.end() != it) {
    return nullptr;
  }
  // This pipe exists, use it to initialize a writer.
  return std::make_shared<PipeWriter<MSG>>(it->second);
}

} // namespace gogort

#endif // GOGO_COMMBUFFER_H
