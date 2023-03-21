//
// Created by Lydett on 3/21/2023.
//

#ifndef GOGO_PROJ_TIME_UTILS_H
#define GOGO_PROJ_TIME_UTILS_H

#include <algorithm>
#include <chrono>
#include <list>
#include <string>

namespace gogort {

using std::chrono::duration;
using std::chrono::high_resolution_clock;

class Timer {
private:
  high_resolution_clock::time_point start_;
  high_resolution_clock::time_point last_;
  std::list<std::pair<uint64_t, std::string>> check_points_;

public:
  Timer() = default;
  void start();
  uint64_t check_and_get_ms(std::string event_name);
};

class RAIITimer {
private:
  high_resolution_clock::time_point start_time_;
  uint64_t *duration_ms_ = nullptr;

  // Start the timer.
  void start();
  // Stop the timer.
  void stop();

public:
  explicit RAIITimer(uint64_t *duration_ms);
  ~RAIITimer();
};

} // namespace gogort

#endif // GOGO_PROJ_TIME_UTILS_H
