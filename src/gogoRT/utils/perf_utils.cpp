//
// Created by Yuting Xie
// 23-3-22

#include "perf_utils.h"
#include "Recorder.h"

#include <cassert>
#include <utility>

namespace gogort {

// If on Linux
#ifdef __linux__

bool PerfMonitor::init() {
  // rf_p_ points to read_buf_, for result reading
  rf_p_ = (ReadFormat *)read_buf_;

  auto make_perf_ctx = [](PerfContext &ctx, std::string name, uint32_t type,
                          uint64_t config, long leader_fd = -1) {
    memset(&ctx.attr, 0, sizeof(struct perf_event_attr));
    ctx.name = name;
    ctx.attr.type = type;
    ctx.attr.config = config;
    ctx.attr.size = sizeof(ctx.attr);
    ctx.attr.disabled = 1;
    ctx.attr.exclude_kernel = 1;
    ctx.attr.exclude_hv = 1;
    ctx.attr.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
    // Only monitor current thread
    ctx.attr.pinned = 1;
    ctx.fd = syscall(__NR_perf_event_open, &ctx.attr, 0, -1, leader_fd, 0);
    // id is different from fd!!!
    ioctl(ctx.fd, PERF_EVENT_IOC_ID, &(ctx.id));
    return true;
  };

  // cycles
  make_perf_ctx(perf_ctx_[0], "cycles", PERF_TYPE_HARDWARE,
                PERF_COUNT_HW_CPU_CYCLES);

  // instructions
  make_perf_ctx(perf_ctx_[1], "instructions", PERF_TYPE_HARDWARE,
                PERF_COUNT_HW_INSTRUCTIONS, perf_ctx_[0].fd);

  // cache references
  make_perf_ctx(perf_ctx_[2], "cache-references", PERF_TYPE_HARDWARE,
                PERF_COUNT_HW_CACHE_REFERENCES, perf_ctx_[0].fd);

  // cache misses
  make_perf_ctx(perf_ctx_[3], "cache-misses", PERF_TYPE_HARDWARE,
                PERF_COUNT_HW_CACHE_MISSES, perf_ctx_[0].fd);

  ioctl(perf_ctx_[0].fd, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
  ioctl(perf_ctx_[0].fd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

  // Start timer also
  timer_.start();

  return true;
}

bool PerfMonitor::start() {
  init();
  // Start all counters
  if (is_running_) {
    assert(false && "PerfMonitor is already running");
  }
  is_running_ = true;
  return true;
}

bool PerfMonitor::stop_and_record(std::string event) {
  auto duration_ms = timer_.get_ms();

  // Stop all counters
  for (auto &ctx : perf_ctx_) {
    ioctl(ctx.fd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
  }

  // Read to buffer, leader fd is the first one
  int leader_fd = perf_ctx_[0].fd;
  ioctl(leader_fd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
  read(leader_fd, read_buf_, sizeof(read_buf_));

  // Seems horrible, but there wont be too many counters
  for (int i = 0; i < rf_p_->nr; ++i) {
    for (auto &ctx : perf_ctx_) {
      if (rf_p_->values[i].id == ctx.id) {
        ctx.value = rf_p_->values[i].value;
        break;
      }
    }
  }

  // Calculate CPU utilization
  uint64_t cycles = perf_ctx_[0].value;
  uint64_t instructions = perf_ctx_[1].value;
  uint64_t cache_references = perf_ctx_[2].value;
  uint64_t cache_misses = perf_ctx_[3].value;
  double cpu_utilization = (double)instructions / (double)cycles;
  double cache_miss_rate = (double)cache_misses / (double)cache_references;
  uint64_t total_memory_accesses = cache_references + cache_misses;

  // Record results.
  Recorder::Instance()->Append(event, Recorder::kPoint, cpu_utilization,
                               "cpu_utilization");
  Recorder::Instance()->Append(event, Recorder::kPoint, total_memory_accesses,
                               "total_memory_accesses");
  Recorder::Instance()->Append(event, Recorder::kPoint, duration_ms,
                               "end_after_ms");

  // Clean up the counters
  for (auto &ctx : perf_ctx_) {
    close(ctx.fd);
  }

  is_running_ = false;
  return true;
}

// Else if on Mac
#elif __APPLE__

bool PerfMonitor::start() {
  timer_.start();
  return true;
}

bool PerfMonitor::stop_and_record(std::string event) {
  auto elapsed_ms = timer_.get_ms_and_reset();
  return Recorder::Instance()->Append(std::move(event), Recorder::kPoint,
                                      elapsed_ms, "end_after_ms");
}

// Else if on Windows
#elif _WIN32

#endif

} // namespace gogort