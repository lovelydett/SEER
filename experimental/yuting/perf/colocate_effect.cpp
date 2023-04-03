//
// Created by Yuting Xie
// 23-4-3

#include <cassert>
#include <cstdio>
#include <thread>
#include <vector>

int main() {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(1, &cpuset);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

  std::thread t([]() {
    // Set priority
    struct sched_param param;
    param.sched_priority = 99;
    auto ret = pthread_setschedparam(pthread_self(), SCHED_RR, &param);
    assert(ret == 0);
    // Set affinity
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    assert(ret == 0);
    while (true) {
      printf("A\n");
      printf("\tA\n");
    }
  });

  std::thread t2([]() {
    // Set priority
    struct sched_param param;
    param.sched_priority = 98;
    auto ret = pthread_setschedparam(pthread_self(), SCHED_RR, &param);
    assert(ret == 0);
    // Set affinity
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    assert(ret == 0);
    while (true) {
      printf("\t\tB\n");
    }
  });

  t.join();
  t2.join();
}