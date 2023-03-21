//
// Created by Lydett on 3/21/2023.
//

#ifndef GOGO_PROJ_TIMER_H
#define GOGO_PROJ_TIMER_H

#include <chrono>

namespace gogort {

using std::chrono::duration;
using std::chrono::high_resolution_clock;

class Timer {
private:
  high_resolution_clock::time_point start_time_;
  uint64_t *duration_ms_ = nullptr;

  // Start the timer.
  void start();
  // Stop the timer.
  void stop();

public:
  explicit Timer(uint64_t *duration_ms);
  ~Timer();
};

} // namespace gogort

#endif // GOGO_PROJ_TIMER_H
