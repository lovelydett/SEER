//
// Created by Yuting Xie
// 23-3-23
// 引入必要的头文件
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>

// Matmul implementations
double A[500][500], B[500][500], C[500][500];

void matmul1() {
  for (int i = 0; i < 500; i++) {
    for (int j = 0; j < 500; j++) {
      for (int k = 0; k < 500; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

void matmul2() {
  for (int i = 0; i < 500; i++) {
    for (int k = 0; k < 500; k++) {
      for (int j = 0; j < 500; j++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

void matmul3() {
  for (int k = 0; k < 500; k++) {
    for (int i = 0; i < 500; i++) {
      for (int j = 0; j < 500; j++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

void matmul4() {
  for (int k = 0; k < 500; k++) {
    for (int j = 0; j < 500; j++) {
      for (int i = 0; i < 500; i++) {
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

void func(int priority) {
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
  io_func();
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

int main() {
  // Spawn threads
  const int N = 100;
  std::vector<std::thread> threads;
  threads.reserve(N);
  for (int i = 0; i < N; ++i) {
    std::thread t(func, i % 100);
    threads.push_back(std::move(t));
  }

  // Join threads
  for (auto &t : threads) {
    t.join();
  }

  std::cout << "Done" << std::endl;

  return 0;
}