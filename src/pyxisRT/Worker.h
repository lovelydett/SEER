// A worker loads a specific dynamic library as task
// Created by Yuting Xie
// 23-4-28

#ifndef SEER_WORKER_H
#define SEER_WORKER_H

#include <dlfcn.h>
#include <functional>
#include <memory>
#include <string>

namespace pyxis {

class Worker {
private:
  std::shared_ptr<std::function<void(void)>> task_;

private:
  bool init_shared_library();

public:
  Worker(const std::string &lib_path);
};

} // namespace pyxis

#endif // SEER_WORKER_H
