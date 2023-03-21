//
// Created by Yuting Xie on 22/03/2023.
//

#ifndef GOGO_PROJ_MOCKWORKLOAD_H
#define GOGO_PROJ_MOCKWORKLOAD_H

#include "../../gogoRT/utils/time_utils.h"

namespace task {

using gogort::Timer;

// The IO and computational load is explicitly configurable
class MockWorkload {
protected:
  float io_load_; // Range from 0 to 1
  Timer timer_;

public:
  explicit MockWorkload(float io_load);
  void set_io_load(float io_load);
  virtual void RunFor(uint64_t duration_ms) = 0;
};

class MonteCarloPiWorkload : public MockWorkload {
private:
  // We have a large enough buffer which exceeds cache size.
  double *result_;
  const int CAPACITY;

public:
  explicit MonteCarloPiWorkload(float io_load);
  void RunFor(uint64_t duration_ms) final;
  ~MonteCarloPiWorkload();
};

class MatMulWorkload : public MockWorkload {
private:
  const int N = 1024;
  double *A, *B, *C;

public:
  explicit MatMulWorkload(float io_load);
  void RunFor(uint64_t duration_ms) final;
};

} // namespace task

#endif // GOGO_PROJ_MOCKWORKLOAD_H
