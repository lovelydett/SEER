// Test glog: https://github.com/google/glog
// Created by Yuting Xie on 09/12/2022.

#include <glog/logging.h>

int main(int argc, char *argv[]) {
  // Initialize Google’s logging library.
  google::InitGoogleLogging(argv[0]);

  // ...
  LOG(ERROR) << "Found " << 111 << " cookies";
}