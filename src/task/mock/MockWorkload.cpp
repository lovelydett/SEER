//
// Created by Yuting Xie on 22/03/2023.
//

#include "MockWorkload.h"
#include "../../gogoRT/utils/utils.h"

#include <cassert>
#include <glog/logging.h>

namespace task {

MockWorkload::MockWorkload(float io_load) { set_io_load(io_load); }

void MockWorkload::set_io_load(float io_load) {
  assert(io_load >= 0 && io_load <= 1);
  io_load_ = io_load;
}

MonteCarloPiWorkload::MonteCarloPiWorkload(float io_load)
    : MockWorkload(io_load), CAPACITY(10 * 1024 * 1024) {
  result_ = new double[CAPACITY];
}

// Calculate pi using Monte Carlo method.
void MonteCarloPiWorkload::RunFor(uint64_t duration_ms) {
  int hit = 0, count = 0;
  int pos[2] = {0, CAPACITY - 1}, idx = 0;
  double pi;
  timer_.start();
  while (timer_.get_ms() < duration_ms)
    [[likely]] {
      ++count;
      double x = random_double(-1, 1);
      double y = random_double(-1, 1);
      if (x * x + y * y <= 1) {
        ++hit;
      }
      pi = 4.0 * hit / count;
      if (random_double(0, 1) < io_load_) {
        result_[pos[idx]] = pi;
        idx = 1 - idx; // idx alternates between 0 and 1
      }
    }
  LOG(INFO) << "MonteCarloPiWorkload ends in " << timer_.get_ms()
            << " ms with pi = " << pi;
}

MonteCarloPiWorkload::~MonteCarloPiWorkload() {
  if (result_ != nullptr) [[likely]] {
    delete[] result_;
  }
}

MatMulWorkload::MatMulWorkload(float io_load) : MockWorkload(io_load) {
  A = new double[N * N];
  B = new double[N * N];
  C = new double[N * N];
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      A[i * N + j] = random_double(0, 1);
      B[i * N + j] = random_double(0, 1);
    }
  }
}

void MatMulWorkload::RunFor(uint64_t duration_ms) {
  timer_.get_ms_and_reset();
  while (true) {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        C[i * N + j] = 0;
        for (int k = 0; k < N; ++k) {
          C[i * N + j] += A[i * N + k] * B[k * N + j];
          if (timer_.get_ms() >= duration_ms) [[unlikely]] {
            return;
          }
        }
      }
    }
  }
}

} // namespace task