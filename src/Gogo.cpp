//
// Created by Yuting Xie
// 22-9-13

#include "gogoRT/Dispatcher.h"
#include <glog/logging.h>
#include <iostream>

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::SetStderrLogging(google::GLOG_INFO);

  LOG(INFO) << "Gogo starts";

  auto dispatcher = gogort::Dispatcher::Instance();
  dispatcher->Run();

  LOG(INFO) << "Gogo finishes";
  return 0;
}