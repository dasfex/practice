#ifndef COUNT_IF_H_
#define COUNT_IF_H_

#include <vector>
#include <functional>
#include <algorithm>
#include <limits>
#include <thread>
#include <mutex>
#include <iostream>

std::vector<int64_t> FindIf(int64_t lower_bound, int64_t upper_bound,
                            const std::function<bool(int64_t)>& predicate,
                            uint8_t concurrency = 1) {
  int64_t elementsCount = upper_bound - lower_bound + 1;
  concurrency = std::min(static_cast<uint64_t>(concurrency),
                         static_cast<uint64_t>(elementsCount));
  std::vector<std::thread> threads;
  threads.reserve(concurrency);
  std::mutex mutex;
  std::vector<int64_t> result;
  int64_t blockSize = elementsCount / concurrency;
  int64_t surplus = elementsCount % concurrency;
  for (int64_t blockNumber = 0; blockNumber < concurrency; ++blockNumber) {
    int64_t curBlockSize = blockSize;
    if (surplus > 0) {
      --surplus;
      ++curBlockSize;
    }
    auto function = [&result, &mutex, &predicate, lower_bound, curBlockSize] {
      int64_t upperBound = lower_bound;
      bool isOverflow = false;
      if (upperBound - 1 + curBlockSize ==
          std::numeric_limits<int64_t>::max()) {
        --upperBound;
        isOverflow = true;
      }
      upperBound += curBlockSize;
      for (int64_t number = lower_bound;
           lower_bound <= number && number < upperBound;
           ++number) {
        if (predicate(number)) {
          std::lock_guard<std::mutex> lock(mutex);
          result.push_back(number);
        }
      }
      if (isOverflow && predicate(upperBound)) {
        std::lock_guard<std::mutex> lock(mutex);
        result.push_back(upperBound);
      }
    };
    threads.emplace_back(function);
    lower_bound += curBlockSize;
  }
  for (auto& thread : threads) {
    thread.join();
  }
  return result;
}

#endif  // COUNT_IF_H_
