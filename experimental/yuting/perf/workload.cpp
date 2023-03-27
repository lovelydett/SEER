//
// Created by Yuting Xie
// 23-3-27

#include "workload.h"

void MatMulFunctor_ijk::operator()() {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      for (int k = 0; k < M; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

void MatMulFunctor_ikj::operator()() {
  for (int i = 0; i < M; i++) {
    for (int k = 0; k < M; k++) {
      for (int j = 0; j < M; j++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

void ConvFunctor::operator()() {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      for (int k = 0; k < 7; k++) {
        for (int l = 0; l < 7; l++) {
          for (int m = 0; m < 7; m++) {
            result[i][j][k] += F[i + l][j + m][k] * kernel[l][m][k];
          }
        }
      }
    }
  }
}

void IOFunctor::operator()() {
  for (int i = 0; i < M / 3; i++) {
    std::for_each(vecs.begin(), vecs.end(), [](std::vector<int> &vec) {
      vec.clear();
      for (int i = 0; i < M; i++) {
        vec.push_back(i);
      }
    });
  }
}

void MonteCarloPiFunctor::operator()() {
  int n = 100000000;
  int count = 0;
  for (int i = 0; i < n; i++) {
    double x = (double)rand() / RAND_MAX;
    double y = (double)rand() / RAND_MAX;
    if (x * x + y * y <= 1) {
      count++;
    }
  }
  double pi = 4.0 * count / n;
}