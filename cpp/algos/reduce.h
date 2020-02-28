#ifndef COUNT_IF_H_
#define COUNT_IF_H_

#include <vector>
#include <functional>
#include <thread>

template<typename ElementType>
ElementType Reduce(
    const std::function<ElementType(const ElementType&,
                                    const ElementType&)>& operation,
    const ElementType& neutral, const std::vector<ElementType>& elements,
    uint8_t concurrency = 1) {
  std::vector<std::thread> threads;
  threads.reserve(concurrency);
  std::vector<ElementType> blockResults(concurrency, neutral);
  int blockSize = elements.size() / concurrency;
  int surplus = elements.size() % concurrency;
  int curBegin = 0;
  for (int blockNumber = 0;
       blockNumber < concurrency && curBegin < elements.size();
       ++blockNumber) {
    int curBlockSize = blockSize;
    if (surplus > 0) {
      --surplus;
      ++curBlockSize;
    }
    ElementType& result = blockResults[blockNumber];
    threads.emplace_back([&elements, &result,
                             &operation, curBegin, curBlockSize] {
      for (int i = curBegin; i < curBegin + curBlockSize; ++i) {
        result = operation(result, elements[i]);
      }
    });
    curBegin += curBlockSize;
  }
  for (auto& thread : threads) {
    thread.join();
  }
  ElementType result(neutral);
  for (const auto& x : blockResults) {
    result = operation(result, x);
  }
  return result;
}

#endif  // COUNT_IF_H_
