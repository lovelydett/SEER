//
// Created by Yuting Xie
// 23-3-27

#ifndef GOGO_PROJ_WORKLOAD_H
#define GOGO_PROJ_WORKLOAD_H

#include <array>
#include <string>
#include <vector>

#define M (1000)

// Base class
struct Workload {
  std::string name_;
  virtual void operator()() = 0;
};

// Mattrix multiplication
struct MatMulFunctor_ijk : public Workload {
  double A[M][M], B[M][M], C[M][M];
  void operator()();
};
static struct MatMulFunctor_ijk matmul_ijk;

struct MatMulFunctor_ikj : public Workload {
  double A[M][M], B[M][M], C[M][M];
  void operator()();
};
static struct MatMulFunctor_ikj matmul_ikj;

// Convolution
struct ConvFunctor : public Workload {
  double F[M][M][7];
  double kernel[7][7][7];
  double result[M][M][7];
  void operator()();
};
static struct ConvFunctor conv;

// IO intensive
struct IOFunctor : public Workload {
  std::array<std::vector<int>, M> vecs;
  void operator()();
};
static struct IOFunctor io;

// MonteCarloPi
struct MonteCarloPiFunctor : public Workload {
  void operator()();
};
static struct MonteCarloPiFunctor monte_carlo_pi;

#endif // GOGO_PROJ_WORKLOAD_H
