#pragma once

#include <mutex>
#include <condition_variable>

class RWLock {
public:
    template <class Func>
    void Read(Func func) {
        std::unique_lock<std::mutex> lock(global_);
        cv_.wait(lock, [this]() {
          return !has_writer_;
        });
        ++blocked_readers_;
        lock.unlock();
        try {
            func();
        } catch (...) {
            EndRead();
            throw;
        }
        EndRead();
    }

    template <class Func>
    void Write(Func func) {
        std::unique_lock<std::mutex> lock(global_);
        cv_.wait(lock, [this]() {
            return !has_writer_;
        });
        has_writer_ = true;
        cv_.wait(lock, [this]() {
            return blocked_readers_ == 0;
        });
        lock.unlock();
        func();
        lock.lock();
        has_writer_ = false;
        cv_.notify_all();
    }

private:
    std::mutex global_;
    std::condition_variable cv_;
    bool has_writer_ = false;
    int blocked_readers_ = 0;

    void EndRead() {
        std::unique_lock<std::mutex> lock(global_);
        --blocked_readers_;
        if (blocked_readers_ == 0) {
            cv_.notify_all();
        }
    }
};
