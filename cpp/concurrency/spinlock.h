#pragma once

#include <atomic>
#include <thread>

class SpinLock {
public:
    void Lock() {
        while (flag_.test_and_set()) {
          std::this_thread::yield();
        }
    }

    void Unlock() {
        flag_.clear();
    }

private:
    std::atomic_flag flag_{false};
};
