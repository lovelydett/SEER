//
// Created by Yuting Xie on 22/03/2023.
//

#ifndef GOGO_PROJ_RECORDER_H
#define GOGO_PROJ_RECORDER_H

#include <fstream>
#include <string>
#include <unordered_map>

namespace gogort {

// Recorder records two types of events: 1.time point events 2.duration events
class Recorder {
private:
  std::ofstream fout_point_;
  std::ofstream fout_duration_;
  static Recorder instance_;
  bool is_init_ = false;
  // Consider most events happen over time, use map to avoid memory release.
  std::unordered_map<std::string, uint64_t> duration_events_;
  Recorder();

public:
  enum RecordType { kPoint = 0, kDuration };
  static Recorder *Instance();
  bool Append(std::string event, RecordType type, uint64_t value,
              std::string explain);
  ~Recorder();
};

} // namespace gogort

#endif // GOGO_PROJ_RECORDER_H
