//
// Created by Yuting Xie on 20/02/2023.
//

#include "RiskManager.h"

namespace gogort {
RiskManager *RiskManager::instance_ = nullptr;
std::mutex RiskManager::instance_lock_ = std::mutex();

RiskManager *RiskManager::Instance() {
  if (instance_ == nullptr) {
    std::lock_guard<std::mutex> lock_guard(instance_lock_);
    if (instance_ == nullptr) {
      instance_ = new RiskManager();
    }
  }
  return instance_;
}

} // namespace gogort