//
// Created by Yuting Xie on 22/03/2023.
//

#include "Recorder.h"
#include "time_utils.h"
#include "utils.h"

#include <cassert>
#include <future>
#include <thread>

namespace gogort {

Recorder Recorder::instance_;
std::mutex Recorder::mtx_;

Recorder::Recorder() {
  auto now_ms = Timer::now_ms();
  std::to_string(now_ms);
  fout_point_.open("../../record/event_point_" + std::to_string(now_ms) +
                   ".csv");
  fout_duration_.open("../../record/event_duration_" + std::to_string(now_ms) +
                      ".csv");
  assert(fout_point_.is_open());
  assert(fout_duration_.is_open());

  /* Record file format:
   * [point events]    tid,time_ms,event,value,explain
   * [duration events] tid,start_ms,end_ms,duration_ms,event,value,explain
   * */
  fout_point_ << "core,tid,time_ms,event,value,explain\n";
  fout_duration_
      << "core,tid,start_ms,end_ms,duration_ms,event,value,explain\n";
}

Recorder *Recorder::Instance() { return &instance_; }

bool Recorder::Append(const std::string event, const RecordType type,
                      const uint64_t value, const std::string explain) {
  // Get current core
  auto core = sched_getcpu();
  auto tid = std::this_thread::get_id();
  auto tid_uint32 = *(uint32_t *)&tid;

  auto async_file_output = [&](const std::string event, const RecordType type,
                               const uint64_t value, const std::string explain,
                               const int core, const int tid) {
    set_thread_affinity(pthread_self(), 15);
    // Todo(yuting): make sure no ',' in event string.
    auto now_ms = Timer::now_ms();

    std::lock_guard<std::mutex> lockGuard(mtx_);

    if (type == kPoint) {
      fout_point_ << core << "," << tid << ',' << now_ms << ',' << event << ','
                  << value << ',' << explain << '\n';
    } else if (type == kDuration) {
      if (!duration_events_.contains(event)) [[unlikely]] {
        duration_events_.insert({event, now_ms});
      }
      auto it = duration_events_.find(event);
      assert(it != duration_events_.end());
      if (it->second != 0) {
        // Means recording ending now.
        fout_duration_ << core << "," << tid << ',' << it->second << ','
                       << now_ms << ',' << now_ms - it->second << ',' << event
                       << ',' << value << ',' << explain << '\n';
      } else {
        // Means recording starting now.
        it->second = now_ms;
      }
    } else {
      assert(false && "Unknown record type");
    }
  };

  std::make_shared<std::thread>(async_file_output, event, type, value, explain,
                                core, tid_uint32)
      ->detach();

  return true;
}

Recorder::~Recorder() {
  fout_duration_.close();
  fout_point_.close();
}

} // namespace gogort