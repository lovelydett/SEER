//
// Created by Yuting Xie
// 22-12-5

#ifndef GOGO_UTILS_H
#define GOGO_UTILS_H

#include <atomic>
#include <random>
#include <thread>

// Define some integer types.
typedef int int32;
typedef unsigned uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef short int16;
typedef unsigned short uint16;

typedef uint16 gogo_id_t;

gogo_id_t get_next_uuid();
uint64 get_current_timestamp_ms();

// Thread utils
void set_thread_priority(pthread_t tid, int priority);
void set_thread_affinity(pthread_t tid, int core);
int get_thread_priority(pthread_t tid);

// Random functions
int random_int();
double random_double(double lb = 0., double ub = 1.);
double random_exponential(double lambada);

#define SAFE_FREE(ptr)                                                         \
  if (ptr != nullptr) {                                                        \
    free(ptr);                                                                 \
    ptr = nullptr;                                                             \
  }

#define SAFE_DELETE(ptr)                                                       \
  if (ptr != nullptr) {                                                        \
    delete (ptr);                                                              \
    ptr = nullptr;                                                             \
  }

#endif // GOGO_UTILS_H