//
// Created by Yuting Xie
// 23-3-27

#include <cassert>
#include <chrono>
#include <cstdio>
#include <thread>
#include <vector>

void dead_loop(int cpu_id) {
  // Set CPU affinity
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cpu_id, &cpuset);
  auto ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
  assert(ret == 0);

  // Set priority
  struct sched_param param;
  param.sched_priority = sched_get_priority_max(SCHED_RR);
  ret = pthread_setschedparam(pthread_self(), SCHED_RR, &param);
  assert(ret == 0);

  auto start = std::chrono::high_resolution_clock::now();
  while (true) {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    if (duration.count() > 30000) {
      printf("CPU %d: %lu ms\n", cpu_id, duration.count());
      break;
    }
  }
}

void dead_loop_ordered_io(int cpu_id) {
  // Set CPU affinity
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cpu_id, &cpuset);
  auto ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
  assert(ret == 0);

  // Set priority
  struct sched_param param;
  param.sched_priority = sched_get_priority_max(SCHED_RR);
  ret = pthread_setschedparam(pthread_self(), SCHED_RR, &param);
  assert(ret == 0);

  int a[1000][1000];

  while (true) {
    for (int i = 0; i < 1000; i++) {
      for (int j = 0; j < 1000; j++) {
        a[i][j] = i + j;
      }
    }
  }
}

void dead_loop_ordered_comp(int cpu_id, int priority) {
  // Set CPU affinity
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cpu_id, &cpuset);
  auto ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
  assert(ret == 0);

  // Set priority
  struct sched_param param;
  param.sched_priority = priority;
  ret = pthread_setschedparam(pthread_self(), SCHED_RR, &param);
  assert(ret == 0);

  int a[1000][1000];

  while (true) {
    for (int i = 0; i < 1000; i++) {
      for (int j = 0; j < 1000; j++) {
        a[i][j] = i + j;
      }
    }
  }
}

int main() {
  const auto N_CPU = std::thread::hardware_concurrency();
  printf("Dead loop on %d CPUs\n", N_CPU);
  std::vector<std::thread> threads;
  for (int i = 0; i < N_CPU; i++) {
    threads.emplace_back(dead_loop, i);
  }
  for (auto &t : threads) {
    t.join();
  }
  return 0;
}