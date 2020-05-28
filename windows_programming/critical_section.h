#ifndef THREADS_CRITICAL_SECTION_H
#define THREADS_CRITICAL_SECTION_H

#include <windows.h>

#include <functional>

class ConditionalVariable;

class CriticalSection {
 public:
  CriticalSection() {
    if (!InitializeCriticalSectionAndSpinCount(&critical_section_,
                                               0x00000400)) {
      throw std::runtime_error("Can't initialize critical section!");
    }
  }

  ~CriticalSection() {
    DeleteCriticalSection(&critical_section_);
  }

  void Run(const std::function<void()>& callback) {
    EnterCriticalSection(&critical_section_);
    callback();
    LeaveCriticalSection(&critical_section_);
  }

  friend class ConditionalVariable;

 public:
  CRITICAL_SECTION critical_section_;
};

#endif // THREADS_CRITICAL_SECTION_H
