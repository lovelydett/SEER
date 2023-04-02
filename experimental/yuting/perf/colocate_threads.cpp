// "cat /proc/sys/kernel/sched_rr_timeslice_ms" to see the default value of
// SCHED_RR policy.
// Created by Yuting Xie 23-4-2

#include <cassert>
#include <cstdio>
#include <memory>
#include <thread>
#include <vector>

static std::unique_ptr<std::thread> thread1 = nullptr, thread2 = nullptr;

pthread_t pthread1, pthread2;

enum Priority {
  kLow = 97,
  kMedium = 98,
  kHigh = 99,
};

void set_priority(pthread_t tid, int priority) {
  // Set priority
  struct sched_param param;
  param.sched_priority = priority;
  auto ret = pthread_setschedparam(tid, SCHED_RR, &param);
  assert(ret == 0);
}

void set_core(pthread_t tid, int core) {
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(core, &cpu_set);
  auto ret = pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpu_set);
  assert(ret == 0);
}

int get_priority(pthread_t tid) {
  struct sched_param param;
  int policy;
  auto ret = pthread_getschedparam(tid, &policy, &param);
  assert(ret == 0);
  return param.sched_priority;
}

void monte_carlo_pi() {
  set_priority(pthread_self(), kHigh);
  auto last = std::chrono::high_resolution_clock::now();
  uint64_t count = 0, total = 0, last_total = 0;
  for (;;) {
    total++;
    double x = (double)rand() / RAND_MAX;
    double y = (double)rand() / RAND_MAX;
    if (x * x + y * y <= 1) {
      count++;
    }
    auto now = std::chrono::high_resolution_clock::now();
    if (now - last > std::chrono::milliseconds(100)) {
      last = now;
      printf("Total = %lu, pi = %.5f, count_local = %lu\n", total,
             4. * count / total, (total - last_total));
      last_total = total;
      // Get a random integer in 0 to 9
      int preempt = rand() % 10 < 3;
      if (false && get_priority(pthread2) == kLow) {
        printf("Preempt, lifting thread2 priority\n");
        set_priority(pthread2, kHigh);
      }
    }
  }
}

void func() {
  set_priority(pthread_self(), kHigh);
  float a1 = 1.5, a2 = 4.8;
  unsigned int total = 0, last_total = 0;
  auto last = std::chrono::high_resolution_clock::now();
  for (;;) {
    ++total;
    a1 *= a2;
    a2 /= a1;
    auto now = std::chrono::high_resolution_clock::now();
    if (now - last > std::chrono::milliseconds(100)) {
      last = now;
      printf("Total = %lu, count_local = %u\n", total, total - last_total);
      last_total = total;
      int preempt = rand() % 100 < 2;
      if (preempt && get_priority(pthread2) == kLow) {
        printf("Preempt, lifting thread2 priority, colocating...\n");
        set_priority(pthread2, kHigh);
      }
    }
  }
}

double A[5000][5000], B[5000][5000], C[5000][5000];

void preempt() {
  set_priority(pthread_self(), kLow);
  while (true) {
    if (get_priority(pthread_self()) != kHigh) {
      continue;
    }
    printf("thread2 lifted to kHigh\n");
    for (int i = 0; i < 5000; i++) {
      for (int j = 0; j < 5000; j++) {
        for (int k = 0; k < 5000; k++) {
          C[i][j] += A[i][k] * B[k][j];
        }
      }
      // printf("P");
    }
    printf("thread2 diving back to kLow\n");
    set_priority(pthread_self(), kLow);
  }
}

void matmul_base() {
  set_priority(pthread_self(), kHigh);
  unsigned long total = 0, last_total = 0;
  auto last = std::chrono::high_resolution_clock::now();
  unsigned long total_normal = 0, total_preempt = 0;
  int normal_round = 0, preempt_round = 0;

  while (true) {
    for (int i = 0; i < 5000; i++) {
      for (int j = 0; j < 5000; j++) {
        for (int k = 0; k < 5000; k++) {
          C[i][j] += A[i][k] * B[k][j];
          ++total;
          auto now = std::chrono::high_resolution_clock::now();
          if (now - last > std::chrono::milliseconds(100)) {
            last = now;
            auto diff = total - last_total;
            last_total = total;
            if (get_priority(pthread2) == kHigh) {
              ++preempt_round;
              total_preempt += diff;
            } else {
              ++normal_round;
              total_normal += diff;
            }
            printf("Total = %lu, freq_normal = %.2f, freq_preempt = %.2f\n",
                   total, total_normal * 1.0 / normal_round,
                   total_preempt * 1.0 / preempt_round);

            int preempt = rand() % 100 < 5;
            if (preempt && get_priority(pthread2) == kLow) {
              printf("Preempt, lifting thread2 priority, colocating...\n");
              set_priority(pthread2, kHigh);
            }
          }
        }
      }
    }
  }
}

void matmul_colocate() {
  set_priority(pthread_self(), kLow);
  unsigned long total = 0, last_total = 0;
  while (true) {
    if (get_priority(pthread_self()) != kHigh) {
      continue;
    }
    printf("thread2 lifted to kHigh\n");
    auto last = std::chrono::high_resolution_clock::now();
    int round = 0;
    for (int i = 0; i < 5000; i++) {
      for (int j = 0; j < 5000; j++) {
        for (int k = 0; k < 5000; k++) {
          total++;
          C[i][j] += A[i][k] * B[k][j];
          auto now = std::chrono::high_resolution_clock::now();
          if (now - last > std::chrono::milliseconds(100)) {
            round++;
            last = now;
            printf("Total = %lu, freq = %.2f\n", total, total / (float)round);
            last_total = total;
          }
        }
      }
    }
    printf("thread2 diving back to kLow\n");
    set_priority(pthread_self(), kLow);
  }
}

// Count computation times rather than time
double AA[500][500], BB[500][500], CC[500][500];
void matmul2_base() {
  set_priority(pthread_self(), kHigh);
  auto start = std::chrono::high_resolution_clock::now();
  auto last = std::chrono::high_resolution_clock::now();
  auto preempt_timer = std::chrono::high_resolution_clock::now();
  while (true) {
    for (int i = 0; i < 500; i++) {
      for (int j = 0; j < 500; j++) {
        for (int k = 0; k < 500; k++) {
          CC[i][j] += AA[i][k] * BB[k][j];
        }
      }
    }
    auto now = std::chrono::high_resolution_clock::now();
    auto total_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start)
            .count();
    auto last_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - last)
            .count();
    last = now;
    printf("Base time used = %llu, speed = %.2f/sec\n", last_ms,
           1000.0 / last_ms);
    if (now - preempt_timer > std::chrono::milliseconds(100)) {
      bool preempt = rand() % 100 < 5;
      if (preempt && get_priority(pthread2) == kLow) {
        printf("Preempt, lifting thread2 priority, colocating...\n");
        set_priority(pthread2, kHigh);
      }
    }
  }
}
void matmul2_colocate() {
  set_priority(pthread_self(), kLow);
  while (true) {
    if (get_priority(pthread_self()) != kHigh) {
      continue;
    }
    printf("thread2 lifted to kHigh\n");
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 500; i++) {
      for (int j = 0; j < 500; j++) {
        for (int k = 0; k < 500; k++) {
          CC[i][j] += AA[i][k] * BB[k][j];
        }
      }
    }
    auto now = std::chrono::high_resolution_clock::now();
    auto last_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start)
            .count();
    printf("\t\t\t\t\t\tColocate mission time = %llu, speed = %.2f/sec\n",
           last_ms, 1000.0 / last_ms);
    printf("thread2 diving back to kLow\n");
    set_priority(pthread_self(), kLow);
  }
}

void colocate_perf_comp(unsigned long long *res) {
  set_priority(pthread_self(), kHigh);
  float x = 1.5, y = 2.5;
  auto start = std::chrono::high_resolution_clock::now();
  for (;;) {
    ++(*res);
    x /= y;
    y /= x;
    auto now = std::chrono::high_resolution_clock::now();
    if (now - start > std::chrono::seconds(1)) {
      break;
    }
  }
}

void colocate_perf_memory_same(unsigned long long *res) {
  set_priority(pthread_self(), kHigh);
  std::vector<int> v;
  auto start = std::chrono::high_resolution_clock::now();
  for (;;) {
    ++(*res);
    if (v.size() < 1000000) {
      v.push_back(1);
    } else {
      v.clear();
    }
    auto now = std::chrono::high_resolution_clock::now();
    auto last_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start)
            .count();
    if (last_ms > 1000) {
      printf("\t\t\t\tEnding after %lu ms\n", last_ms);
      break;
    }
  }
}

int main() {
  // Spawn threads
  //  pthread_create(&pthread1, NULL, (void *(*)(void *))matmul2_base, NULL);
  //  pthread_create(&pthread2, NULL, (void *(*)(void *))matmul2_colocate,
  //  NULL);
  //
  //  set_core(pthread1, 0);
  //  set_core(pthread2, 0);
  //
  //  pthread_join(pthread1, NULL);
  //  pthread_join(pthread2, NULL);

  // Perf test
  set_core(pthread_self(), 0);
  set_priority(pthread_self(), kHigh);
  unsigned long long baseline = 0;
  // Cache warmup
  colocate_perf_memory_same(&baseline);
  set_core(pthread_self(), 1);

  printf("Baseline: %llu\n", baseline);

  for (int n = 2; n <= 64; ++n) {
    unsigned long long *res = new unsigned long long[n];
    for (int i = 0; i < n; i++) {
      res[i] = 0;
    }
    pthread_t *pthreads = new pthread_t[n];
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
      pthread_create(&pthreads[i], NULL,
                     (void *(*)(void *))colocate_perf_memory_same, &res[i]);
      set_core(pthreads[i], 0);
    }
    // Join threads
    for (int i = 0; i < n; i++) {
      pthread_join(pthreads[i], NULL);
    }
    auto now = std::chrono::high_resolution_clock::now();
    auto last_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start)
            .count();

    // Collect results
    double performance = 0, avg = 0.f;
    for (int i = 0; i < n; i++) {
      avg = (avg * i + res[i]) / (i + 1);
      performance += res[i] / (double)baseline;
      // printf("Thread %d: %llu\n", i, res[i]);
    }
    printf("%d threads colocate, ends in %llu ms, average operations = %.2f, "
           "perf = %.2f%%\n",
           n, last_ms, avg, performance * 100.0);
  }
}