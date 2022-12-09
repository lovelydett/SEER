//
// Created by Yuting Xie
// 22-9-13

#include "gogoRT/Dispatcher.h"
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "Gogo starts." << std::endl;

  auto dispatcher = gogort::Dispatcher::Instance();
  dispatcher->Run();

  std::cout << "Gogo finishes." << std::endl;
  return 0;
}