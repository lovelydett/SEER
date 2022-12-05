//
// Created by Yuting Xie
// 22-9-13

#ifndef GOGO_TASK_H
#define GOGO_TASK_H

#include <memory>

namespace gogort {

// Represent null type, facilitate variant template arguments
class NullClass {};

// The original template class that supports at most 4 types of input messages
template <class MSG0 = NullClass, class MSG1 = NullClass,
          class MSG2 = NullClass, class MSG3 = NullClass>
class Task {
  virtual bool Deal(const std::shared_ptr<MSG0> &,
                    const std::shared_ptr<MSG1> &,
                    const std::shared_ptr<MSG2> &,
                    const std::shared_ptr<MSG3> &) = 0;
};

// Task that takes 0 message
template <> class Task<NullClass, NullClass, NullClass, NullClass> {
  virtual bool Deal() = 0;
};

// Task that takes 1 message
template <class MSG0> class Task<MSG0, NullClass, NullClass, NullClass> {
  virtual bool Deal(const std::shared_ptr<MSG0> &) = 0;
};

// Task that takes 2 messages
template <class MSG0, class MSG1> class Task<MSG0, MSG1, NullClass, NullClass> {
  virtual bool Deal(const std::shared_ptr<MSG0> &,
                    const std::shared_ptr<MSG1> &) = 0;
};

// Task that takes 3 messages
template <class MSG0, class MSG1, class MSG2>
class Task<MSG0, MSG1, MSG2, NullClass> {
  virtual bool Deal(const std::shared_ptr<MSG0> &,
                    const std::shared_ptr<MSG1> &,
                    const std::shared_ptr<MSG2> &) = 0;
};

} // namespace gogort

#endif // GOGO_TASK_H
