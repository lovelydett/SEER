// For active pipe reading, not in use now
// Created by Yuting Xie
// 22-12-6

#ifndef GOGO_PIPEREADER_H
#define GOGO_PIPEREADER_H

#include <string>

namespace gogort {

template <class MSG>
class PipeReader {
private:
  std::string pipe_name_;

public:
  PipeReader() = delete;
  PipeReader(const PipeReader &) = delete;
  PipeReader &operator=(const PipeReader &) = delete;
  PipeReader(PipeReader &&) = delete;
  PipeReader(std::string pipe_name) : pipe_name_(pipe_name) {}
  virtual ~PipeReader() = default;

};

} // namespace gogort

#endif // GOGO_PIPEREADER_H
