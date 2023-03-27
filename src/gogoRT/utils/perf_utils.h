//
// Created by Yuting Xie
// 23-3-22

#ifndef GOGO_PROJ_PERF_UTILS_H
#define GOGO_PROJ_PERF_UTILS_H

#include "time_utils.h"

#include <string>

// If on Linux
#ifdef __linux__

#define _GNU_SOURCE
#include <asm/unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <linux/hw_breakpoint.h>
#include <linux/perf_event.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <array>

#endif

namespace gogort {

#ifdef __linux__

class PerfMonitor {
private:
  typedef struct PerfResultReadFormat {
    uint64_t nr;
    struct {
      uint64_t value;
      uint64_t id;
    } values[];
  } ReadFormat;

  typedef struct PerfContext {
    std::string name;
    struct perf_event_attr attr;
    long fd;
    uint64_t id;
    uint64_t value;
    ReadFormat *result;
  } PerfContext;

private:
  bool is_running_ = false;
  std::array<PerfContext, 10> perf_ctx_;
  char read_buf_[4096];
  ReadFormat *rf_p_;
  Timer timer_;

private:
  bool init();

private:
public:
  PerfMonitor() = default;
  bool start();
  bool stop_and_record(std::string event);
};

// Else if on Mac
#elif __APPLE__

class PerfMonitor {
private:
  Timer timer_;

public:
  PerfMonitor() = default;
  ~PerfMonitor() = default;
  bool start();
  bool stop_and_record(std::string event);
};

// Else if on Windows
#elif _WIN32

class PerfMonitor {
private:
public:
  PerfMonitor() = default;
  bool start() { return true; }
  bool stop_and_record(std::string event) { return true; }
};

#endif

} // namespace gogort

#endif // GOGO_PROJ_PERF_UTILS_H
