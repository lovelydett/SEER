//
// Created by Yuting Xie
// 22-9-13

#ifndef GOGO_COMMBUFFER_H
#define GOGO_COMMBUFFER_H

#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>

#include "Pipe.h"
#include "PipeReader.h"
#include "PipeWriter.h"

namespace gogort {

class CommBuffer {
private:
  CommBuffer() = default;
  bool init_config(const std::string config_path);

private:
  bool is_init_ = false;
  static CommBuffer *instance_;
  static std::mutex mtx_;
  // Pipe name to pipe
  std::unordered_map<std::string, std::shared_ptr<Pipe>> name_to_pipe_;

public:
  static CommBuffer *Instance();

  // Tell CommBuffer you need a reader
  template <class MSG>
  std::optional<PipeReader<MSG>> AcquireReader(const std::string pipe_name) {
    return std::nullopt;
  }

  // CommBuffer will prepare the messages in pipe-of-interest for the task.
  template <class MSG>
  std::optional<PipeWriter<MSG>> AcquireWriter(const std::string pipe_name) {
    auto it = name_to_pipe_.find(pipe_name);
    if (name_to_pipe_.end() != it) {
      return std::nullopt;
    }
    // This pipe exists, use it to initialize a writer.
    return PipeWriter<MSG>(it->second);
  }
};

} // namespace gogort

#endif // GOGO_COMMBUFFER_H
