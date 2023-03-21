//
// Created by Lydett on 3/21/2023.
//

#include "time_utils.h"

#include <cassert>

namespace gogort {

using std::chrono::duration_cast;

void Timer::start() {
  start_ = high_resolution_clock::now();
  last_ = high_resolution_clock::now();
  is_started_ = true;
}
uint64_t Timer::get_ms_and_check(const std::string event_name) {
  auto elapse_ms = get_ms();
  check_points_.emplace_back(elapse_ms, event_name);
  return elapse_ms;
}
uint64_t Timer::get_ms() {
  auto now = high_resolution_clock::now();
  auto elapse_ms =
      duration_cast<duration<uint64_t, std::ratio<1, 1000>>>(now - last_)
          .count();
  last_ = now;
  return elapse_ms;
}
uint64_t Timer::get_ms_and_reset() {
  auto elapse_ms = get_ms();
  check_points_.clear();
  is_started_ = false;
  return elapse_ms;
}

bool Timer::is_started() const { return is_started_; }

RAIITimer::RAIITimer(uint64_t *duration_ms) : duration_ms_(duration_ms) {
  start();
}
RAIITimer::~RAIITimer() { stop(); }

// Start the timer.
void RAIITimer::start() { start_time_ = high_resolution_clock::now(); }
// Stop the timer.
void RAIITimer::stop() {
  assert(duration_ms_ != nullptr);
  auto end_time = high_resolution_clock::now();
  *duration_ms_ = duration_cast<duration<uint64_t, std::ratio<1, 1000>>>(
                      end_time - start_time_)
                      .count();
}

} // namespace gogort