#define _GNU_SOURCE
#include <asm/unistd.h>
#include <chrono>
#include <ctime>
#include <errno.h>
#include <inttypes.h>
#include <iostream>
#include <linux/hw_breakpoint.h>
#include <linux/perf_event.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>
#include <vector>

struct read_format {
  uint64_t nr;
  struct {
    uint64_t value;
    uint64_t id;
  } values[];
};

static const int M = 2000;
static double A[M][M];
static double B[M][M];
static double C[M][M];
void matmul1() {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      for (int k = 0; k < M; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

// An IO intensive function
void io_func() {
  std::vector<int> v;
  for (int i = 0; i < 1000000; i++) {
    v.push_back(i);
  }
}

uint64_t get_thread_time_ms(pthread_t tid) {
  clockid_t cid;
  int ret = pthread_getcpuclockid(tid, &cid);
  if (ret != 0) {
    std::cout << "pthread_getcpuclockid error: " << ret << std::endl;
    return -1;
  }
  struct timespec ts;
  ret = clock_gettime(cid, &ts);
  if (ret != 0) {
    std::cout << "clock_gettime error: " << ret << std::endl;
    return -1;
  }

  // Add up seconds and nanoseconds
  auto timestamp_ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

  return timestamp_ms;
}

void do_perf_1(int priority) {
  // set cpu affinity
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(0, &cpuset);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

  // set thread priority
  struct sched_param param;
  param.sched_priority = priority;
  pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);

  uint64_t start_ms = get_thread_time_ms(pthread_self());
  auto start_time = std::chrono::high_resolution_clock::now();
  // matmul1();
  matmul1();
  auto end_time = std::chrono::high_resolution_clock::now();
  uint64_t end_ms = get_thread_time_ms(pthread_self());
  uint64_t thread_time_ms = (end_ms - start_ms);
  uint64_t wall_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                              end_time - start_time)
                              .count();
  std::cout << "tid: " << pthread_self() << ", priority: " << priority
            << ", thread time (ms): " << thread_time_ms
            << "， wall time (ms): " << wall_time_ms
            << "， utilization: " << thread_time_ms * 100.0 / wall_time_ms
            << '%' << std::endl;
}

void do_perf_2(void (*func)(void), int priority) {
  // Set affinity
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(0, &cpuset);
  sched_setaffinity(pthread_self(), sizeof(cpu_set_t), &cpuset);

  // Set priority
  struct sched_param param;
  param.sched_priority = priority;
  sched_setscheduler(pthread_self(), SCHED_RR, &param);

  struct perf_event_attr pea;
  int fd1, fd2, fd3;
  uint64_t id1, id2, id3;
  uint64_t val1, val2, val3;
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

  auto start = std::chrono::high_resolution_clock::now();

  ioctl(fd1, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
  ioctl(fd1, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

  func();

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
    }
  }

  printf("priority: %d, cycle: %lld, page fault: %lld, instruction: %lld, "
         "duration(ms): %lld, "
         "utilization: %.2f\n",
         priority, val1, val2, val3, duration_ms, (double)val3 / val1 * 100);
}

int main(int argc, char *argv[]) {
  printf("Begin\n");
  // Spawn threads
  const int N = 16;
  std::vector<std::thread> threads;
  threads.reserve(2 * N);

  for (int i = 0; i < N; ++i) {
    std::thread t(do_perf_1, i % 100);
    threads.push_back(std::move(t));
  }

  for (int i = N; i < 2 * N; ++i) {
    std::thread t(do_perf_2, matmul1, i % 100);
    threads.push_back(std::move(t));
  }

  // Join threads
  for (auto &t : threads) {
    t.join();
  }

  printf("Done\n");
  return 0;
}