//
// Created by Yuting Xie
// 23-3-28

#include <cassert>
#include <chrono>
#include <cstdio>
#include <thread>
#include <vector>

void monte_carlo_pi(int cpu_id, int priority) {
  printf("tid = %lu starts\n", pthread_self());
  // Set affinity
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

  auto last = std::chrono::high_resolution_clock::now();
  int n_print = 1;
  int count = 0, total = 0;
  for (int i = 0; i < 100000000; i++) {
    total++;
    double x = (double)rand() / RAND_MAX;
    double y = (double)rand() / RAND_MAX;
    if (x * x + y * y <= 1) {
      count++;
    }
    if (total == 10000000 * n_print) {
      auto now = std::chrono::high_resolution_clock::now();
      auto duration_ms =
          std::chrono::duration_cast<std::chrono::milliseconds>(now - last)
              .count();
      last = now;
      n_print++;
      printf("priority: %d, count = %d, pi = %.5f, duration(ms): %lu\n",
             priority, count, 4. * count / total, duration_ms);
    }
  }
  printf("tid = %lu finishes\n", pthread_self());
}

int main() {
  // Spawn threads
  std::vector<std::thread> threads;
  threads.reserve(10);

  for (int i = 0; i < 3; i++) {
    threads.emplace_back(monte_carlo_pi, 0, i + 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  std::for_each(threads.begin(), threads.end(),
                [](std::thread &t) { t.join(); });

  return 0;
}