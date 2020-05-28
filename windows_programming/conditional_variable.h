#define _WIN32_WINNT 0x0600
#ifndef THREADS_CONDITIONAL_VARIABLE_H
#define THREADS_CONDITIONAL_VARIABLE_H

#include "critical_section.h"

class ConditionVariable {
 public:
  ConditionVariable() {
    InitializeConditionVariable(&condition_variable_);
  }

  void notify_one() {
    WakeConditionVariable(&condition_variable_);
  }

  void notify_all() {
    WakeAllConditionVariable(&condition_variable_);
  }

  template<class Pred>
  void wait(CriticalSection& lock, Pred pred = [](){ return false; }) {
    while (!pred()) {
      SleepConditionVariableCS(&condition_variable_,
                               &lock.critical_section_,
                               INFINITE);
    }
  }

 private:
  CONDITION_VARIABLE condition_variable_;
};

#endif // THREADS_CONDITIONAL_VARIABLE_H
