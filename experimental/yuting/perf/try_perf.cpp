//
// Created by Yuting Xie
// 23-3-27

#include "workload.h"

#include <asm/unistd.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <linux/perf_event.h>
#include <memory>
#include <string>
#include <sys/ioctl.h>
#include <thread>

// Different perf tools
void perf_with_thread_time(Workload &workload, int priority, int cpu_core,
                           std::string name, uint64_t *thread_time_ptr);
void perf_with_hw_counters(Workload &workload, int priority, int cpu_core,
                           std::string name, uint64_t *thread_time_ptr);
void perf_with_roofline(Workload &workload, int priority, int cpu_core,
                        std::string name);

// Set thread priority and cpu affinity
void set_thread_priority(pthread_t tid, int priority, int policy = SCHED_RR);
void set_thread_affinity(pthread_t tid, int cpu_core);

// Get time since program start
uint64_t get_program_time_ms();
std::chrono::high_resolution_clock::time_point program_start_time;

// The profiling logic
void do_profiling(int num_core, int num_workload, int max_priority = 99);

// Evaluate the contention effect on one core
void do_profiling_single_core_comp(int num_workload);

// Evaluate the contention effect of different workloads
void do_profiling_single_core_multi_type_workload(int iter_num = 10);

int main() {
  program_start_time = std::chrono::high_resolution_clock::now();
  printf("******* Program begins here ******\n\n");

  //  for (int i = 1; i <= 8; i *= 2) {
  //    do_profiling(i, 16);
  //  }

  //  for (int i = 1; i <= 16; i *= 2) {
  //    do_profiling_single_core_comp(i);
  //  }

  do_profiling_single_core_multi_type_workload(100);

  printf("\n******* Program ends here ******\n");

  return 0;
}

void perf_with_thread_time(Workload &workload, int priority, int cpu_core,
                           std::string name, uint64_t *thread_time_ptr) {
  // set cpu affinity and priority
  set_thread_affinity(pthread_self(), cpu_core);
  set_thread_priority(pthread_self(), priority, SCHED_RR);

  auto get_thread_time_ms = [](pthread_t tid) -> uint64_t {
    clockid_t cid;
    int ret = pthread_getcpuclockid(tid, &cid);
    if (ret != 0) {
      printf("pthread_getcpuclockid error: %d\n", ret);
      return -1;
    }
    struct timespec ts;
    ret = clock_gettime(cid, &ts);
    if (ret != 0) {
      printf("clock_gettime error: %d\n", ret);
      return -1;
    }

    // Add up seconds and nanoseconds
    auto timestamp_ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

    return timestamp_ms;
  };

  uint64_t start_ms = get_thread_time_ms(pthread_self());
  auto start_time = std::chrono::high_resolution_clock::now();
  auto time_since_program_ms = get_program_time_ms();
  workload();
  auto end_time = std::chrono::high_resolution_clock::now();
  uint64_t end_ms = get_thread_time_ms(pthread_self());

  uint64_t thread_time_ms = (end_ms - start_ms);
  uint64_t wall_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                              end_time - start_time)
                              .count();
  printf("core: %d, priority: %d, thread time "
         "(ms): %lu, wall time (ms): "
         "%lu, utilization: %.2f%%\n",
         cpu_core, priority, thread_time_ms, wall_time_ms,
         thread_time_ms * 100.0 / wall_time_ms);

  *thread_time_ptr = wall_time_ms;
}

void set_thread_priority(pthread_t tid, int priority, int policy) {
  struct sched_param param;
  param.sched_priority = priority;
  pthread_setschedparam(tid, policy, &param);
}

void set_thread_affinity(pthread_t tid, int cpu_core) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cpu_core, &cpuset);
  pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpuset);
}

uint64_t get_program_time_ms() {
  auto now = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             now - program_start_time)
      .count();
}

void do_profiling(int num_core, int num_workload, int max_priority,
                  std::string name) {
  printf("--- Num of cores: %d, num of workloads: %d --- \n", num_core,
         num_workload);
  // Create threads
  std::vector<std::thread> threads;
  threads.reserve(num_workload);
  for (int i = 0; i < num_workload; ++i) {
    uint64_t wall_time;
    // Must use std::ref to pass reference to thread (actually bind)
    threads.emplace_back(perf_with_thread_time, std::ref(matmul_ijk),
                         sched_get_priority_max(SCHED_RR), 0,
                         "matmul" + std::to_string(i), &wall_time);
  }
  // Join threads
  for (auto &t : threads) {
    t.join();
  }

  printf("\n");

  return;
}

void do_profiling_single_core_comp(int num_workload) {
  printf("--- Single core perf: %d workloads --- \n", num_workload);
  // Create threads
  std::vector<std::thread> threads;
  threads.reserve(num_workload);

  // Cache wamup
  for (int i = 0; i < 2; ++i) {
    io();
  }

  uint64_t time_ms;
  for (int i = 0; i < num_workload; ++i) {
    threads.emplace_back(perf_with_hw_counters, std::ref(io),
                         sched_get_priority_max(SCHED_RR), 0,
                         "io" + std::to_string(i), &time_ms);
  }
  // Join threads
  for (auto &t : threads) {
    t.join();
  }
  printf("\n");
}

void do_profiling_single_core_multi_type_workload(int iter_num) {
  std::vector<Workload *> workloads;
  std::vector<std::string> names;
  std::vector<uint64_t> baselines;
  std::vector<uint64_t> res1, res2;
  workloads.push_back(&matmul_ijk);
  names.emplace_back("matmul_ijk");
  workloads.push_back(&matmul_ikj);
  names.emplace_back("matmul_ikj");
  workloads.push_back(&io);
  names.emplace_back("io");
  workloads.push_back(&conv);
  names.emplace_back("conv");
  workloads.push_back(&monte_carlo_pi);
  names.emplace_back("monte_carlo_pi");
  baselines.resize(names.size());
  res1.resize(names.size());
  res2.resize(names.size());

  printf("--- Single core multi type workload --- \n");

  //  Cache warmup
  for (auto w : workloads) {
    (*w)();
  }
  for (auto w : workloads) {
    (*w)();
  }

  // First get the single core single workload data
  for (int i = 0; i < workloads.size(); ++i) {
    perf_with_thread_time(*workloads[i], sched_get_priority_max(SCHED_RR), 0,
                          names[i], &baselines[i]);
  }

  for (int n = 1; n <= iter_num; ++n) {
    printf("\nIteration %d/%d\n", n, iter_num);
    // Randomly sample two workloads
    int i1 = rand() % workloads.size();
    int i2 = rand() % workloads.size();

    std::thread t1(perf_with_hw_counters, std::ref(*workloads[i1]),
                   sched_get_priority_max(SCHED_RR), 0, names[i1], &res1[i1]);
    std::thread t2(perf_with_hw_counters, std::ref(*workloads[i2]),
                   sched_get_priority_max(SCHED_RR), 0, names[i2], &res2[i2]);
    t1.join();
    t2.join();

    // Collect results
    printf("--- Result:\n");
    printf("%s:\n%lu -> %lu \t %.2f%% increase.\n", names[i1].c_str(),
           baselines[i1], res1[i1],
           (res1[i1] - baselines[i1]) * 100.0 / baselines[i1]);
    printf("%s:\n%lu -> %lu \t %.2f%% increase.\n", names[i2].c_str(),
           baselines[i2], res2[i2],
           (res2[i2] - baselines[i2]) * 100.0 / baselines[i2]);
    printf("\n");
  }
}

struct read_format {
  uint64_t nr;
  struct {
    uint64_t value;
    uint64_t id;
  } values[];
};

void perf_with_hw_counters(Workload &workload, int priority, int cpu_core,
                           std::string name, uint64_t *thread_time_ptr) {
  // Set affinity and priority
  set_thread_affinity(pthread_self(), cpu_core);
  set_thread_priority(pthread_self(), priority, SCHED_RR);

  struct perf_event_attr pea;
  int fd1, fd2, fd3, fd4, fd5;
  uint64_t id1, id2, id3, id4, id5;
  uint64_t val1, val2, val3, val4, val5;
  char buf[4096];
  struct read_format *rf = (struct read_format *)buf;
  int i;

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_HARDWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_HW_CPU_CYCLES;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd1 = syscall(__NR_perf_event_open, &pea, 0, -1, -1, 0);
  ioctl(fd1, PERF_EVENT_IOC_ID, &id1);

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_SOFTWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_SW_PAGE_FAULTS;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd2 = syscall(__NR_perf_event_open, &pea, 0, -1, fd1 /*!!!*/, 0);
  ioctl(fd2, PERF_EVENT_IOC_ID, &id2);

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_SOFTWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_HW_INSTRUCTIONS;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd3 = syscall(__NR_perf_event_open, &pea, 0, -1, fd1 /*!!!*/, 0);
  ioctl(fd3, PERF_EVENT_IOC_ID, &id3);

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_HARDWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_HW_CACHE_REFERENCES;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd4 = syscall(__NR_perf_event_open, &pea, 0, -1, fd1 /*!!!*/, 0);
  ioctl(fd4, PERF_EVENT_IOC_ID, &id4);

  memset(&pea, 0, sizeof(struct perf_event_attr));
  pea.type = PERF_TYPE_HARDWARE;
  pea.size = sizeof(struct perf_event_attr);
  pea.config = PERF_COUNT_HW_CACHE_MISSES;
  pea.disabled = 1;
  pea.exclude_kernel = 1;
  pea.exclude_hv = 1;
  pea.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;
  fd5 = syscall(__NR_perf_event_open, &pea, 0, -1, fd1 /*!!!*/, 0);
  ioctl(fd5, PERF_EVENT_IOC_ID, &id5);

  auto start = std::chrono::high_resolution_clock::now();

  ioctl(fd1, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
  ioctl(fd1, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

  workload();

  ioctl(fd1, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  read(fd1, buf, sizeof(buf));
  for (i = 0; i < rf->nr; i++) {
    if (rf->values[i].id == id1) {
      val1 = rf->values[i].value;
    } else if (rf->values[i].id == id2) {
      val2 = rf->values[i].value;
    } else if (rf->values[i].id == id3) {
      val3 = rf->values[i].value;
    } else if (rf->values[i].id == id4) {
      val4 = rf->values[i].value;
    } else if (rf->values[i].id == id5) {
      val5 = rf->values[i].value;
    }
  }

  printf("name: %s, cpu: %d, priority: %d, cycle: %lu, page fault: %lu, "
         "instruction: %lu, "
         "wall-clock(ms): %lu, "
         "cache-ref: %lu, "
         "cache-miss: %lu, "
         "roofline: %.3f\n",
         name.c_str(), cpu_core, priority, val1, val2, val3, duration_ms, val4,
         val5, (double)val3 / (double)(val4 + val5));

  *thread_time_ptr = duration_ms;
}