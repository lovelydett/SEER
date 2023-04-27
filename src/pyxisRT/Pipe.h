// A pipe is essentially a named shared memory
// Created by Yuting Xie
// 23-4-28

#ifndef SEER_PIPE_H
#define SEER_PIPE_H

#include "../utils/utils.h"

#include <pthread.h>
#include <string>
#include <sys/shm.h>

namespace pyxis {

class Pipe {
private:
  std::string name_;
  int shm_id_ = -1;
  void *shm_ptr_ = nullptr;
  pthread_mutex_t *mutex_ptr_ = nullptr;
  size_t pipe_size_ = 0;
  uint64_t timestamp_ms_ = 0;

private:
  bool init();
  void lock(); // Blocking lock
  bool try_look();
  void unlock();

public:
  Pipe(const Pipe &pipe); // Pipe is copyable, just pointer-like.
  ~Pipe();
  bool
  Write(const void *data); // Must pass-in the ptr, to copy data within lock.
  bool Read(void *data,
            uint64_t *timestamp_ms); // Must pass-in the ptr, to copy data
                                     // within lock.
  uint64_t GetTimestampMs();
  size_t GetSize();
};

} // namespace pyxis

#endif // SEER_PIPE_H
