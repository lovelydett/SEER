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
  bool is_started_ = false;

public:
  Timer() = default;
  void start();
  // Mark an event and return the time elapsed since last event.
  uint64_t get_ms_and_check(std::string event_name = "<NULL>");
  // Return the time elapsed since last event.
  uint64_t get_ms();
  // Return the time elapsed since last event and reset
  uint64_t get_ms_and_reset();
  [[nodiscard]] bool is_started() const;
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
