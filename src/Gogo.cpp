//
// Created by Yuting Xie
// 22-9-13

#include <iostream>
#include "gogoRT/Dispatcher.h"

int main (int argc, char *argv[]) {
  std::cout << "Gogo started." << std::endl;

  auto dispatcher = gogort::Dispatcher::Instance_();
  dispatcher->Run();

  std::cout << "Gogo finished." << std::endl;
  return 0;
}