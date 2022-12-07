//
// Created by Yuting Xie
// 22-12-7

#ifndef GOGO_TASKFACTORY_H
#define GOGO_TASKFACTORY_H

namespace gogort {

class TaskFactory {
private:
  static TaskFactory *instance_;

private:
  TaskFactory() = default;

public:
  TaskFactory* Instance() {
    if (instance_ == nullptr) {
      instance_ = new TaskFactory();
    }
    return instance_;
  }




};

} // namespace gogort

#endif // GOGO_TASKFACTORY_H
