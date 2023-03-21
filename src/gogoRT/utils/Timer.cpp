//
// Created by Lydett on 3/21/2023.
//

#include "Timer.h"

#include <cassert>

namespace gogort {

using std::chrono::duration_cast;

Timer::Timer(uint64_t *duration_ms) : duration_ms_(duration_ms) { start(); }
Timer::~Timer() { stop(); }

// Start the timer.
void Timer::start() { start_time_ = high_resolution_clock::now(); }
// Stop the timer.
void Timer::stop() {
  assert(duration_ms_ != nullptr);
  auto end_time = high_resolution_clock::now();
  *duration_ms_ = duration_cast<duration<uint64_t, std::ratio<1, 1000>>>(
                      end_time - start_time_)
                      .count();
}

} // namespace gogort