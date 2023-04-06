//
// Created by Yuting Xie
// 2022/12/7

#include "utils.h"

#include <cassert>
#include <chrono>
#include <random>

gogo_id_t get_next_uuid() {
  static std::atomic<uint16> uuid_ = 0;
  uuid_ += 1;
  return uuid_.load();
}

static std::default_random_engine random_engine;

int random_int() { return random_engine(); }

double random_double(double lb, double ub) {
  std::uniform_real_distribution distribution(lb, ub);
  return distribution(random_engine);
}

double random_exponential(double lambda) {
  std::exponential_distribution<double> distribution(lambda);
  return distribution(random_engine);
}

uint64 get_current_timestamp_ms() {
  static auto base = std::chrono::system_clock::now();
  auto now = std::chrono::system_clock::now();
  //  auto duration = now.time_since_epoch();
  auto duration = now - base;
  return std::chrono::duration_cast<std::chrono::milliseconds>(duration)
      .count();
}

void set_thread_priority(pthread_t tid, int priority) {
  struct sched_param param;
  param.sched_priority = priority;
  auto ret = pthread_setschedparam(tid, SCHED_RR, &param);
  assert(ret == 0);
}

void set_thread_affinity(pthread_t tid, int core) {
#ifdef __linux__
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(core, &cpuset);
  auto ret = pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpuset);
  assert(ret == 0);
#endif
}

int get_thread_priority(pthread_t tid) {
  struct sched_param param;
  int policy;
  auto ret = pthread_getschedparam(tid, &policy, &param);
  assert(ret == 0);
  return param.sched_priority;
}

int get_thread_core(pthread_t tid) {
#ifdef __linux__
  cpu_set_t cpuset;
  auto ret = pthread_getaffinity_np(tid, sizeof(cpu_set_t), &cpuset);
  assert(ret == 0);
  for (int i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, &cpuset)) {
      return i;
    }
  }
#endif
  return -1;
}