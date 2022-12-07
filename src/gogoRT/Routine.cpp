//
// Created by Yuting Xie on 03/12/2022.
//

#include "Routine.h"

#include <utility>

namespace gogort {
Routine::Routine(std::function<void()> func) {
  // At generation, set this routine as not finished.
  is_finished_ = false;
  func_ = [&]() {
    func();
    is_finished_ = true;
  };
}
} // namespace gogort