#include <list>

#include <benchmark/benchmark.h>

#include "memory_list.hpp"

namespace {

constexpr std::size_t kSize = 100'000u;

struct Heavy {
  char data[64];
};

template <typename T>
std::list<T> GetStlList(std::size_t n, T t = T()) {
  std::list<T> list;
  for (int i = 0; i < n; ++i) {
    list.push_back(t);
  }
  return list;
}

template <typename T, std::size_t N>
MemList<T, N> GetMemList(T t = T()) {
  MemList<T, N> list;
  for (int i = 0; i < N; ++i) {
    list.PushBack(t);
  }
  return list;
}

template <typename T>
void StlListInsert(std::size_t n, T t = T()) {
  std::list<T> list;
  for (int i = 0; i < n; ++i) {
    list.push_back(t);
  }
}

template <typename T, std::size_t N>
void MemListInsert(T t = T()) {
  MemList<T, N> list;
  for (int i = 0; i < N; ++i) {
    list.PushBack(t);
  }
}

template <typename T>
T StlListFor(const std::list<T>& list) {
  T result = T();
  for (auto begin = list.begin(), end = list.end(); begin != end; ++begin) {
    result = *begin;
  }
  return result;
}

template <typename T>
T MemListFor(MemList<T, kSize>& list) {
  T result = T();
  for (auto begin = list.Begin(), end = list.End(); begin != end; ++begin) {
    result = *begin;
  }
  return result;
}

}  // namespace

static void bmStlListInsertInt(benchmark::State& state) {
  for (auto _ : state) {
    StlListInsert<int>(kSize, 0);
  }
}

static void bmStlListInsertHeavy(benchmark::State& state) {
  for (auto _ : state) {
    StlListInsert<Heavy>(kSize, Heavy{});
  }
}

static void bmMemListInsertInt(benchmark::State& state) {
  for (auto _ : state) {
    MemListInsert<int, kSize>(0);
  }
}

static void bmMemListInsertHeavy(benchmark::State& state) {
  for (auto _ : state) {
    MemListInsert<Heavy, kSize>(Heavy{});
  }
}

static void bmStlListForInt(benchmark::State& state) {
  auto list = GetStlList<int>(kSize, 0);
  for (auto _ : state) {
    StlListFor<int>(list);
  }
}

static void bmMemListForInt(benchmark::State& state) {
  auto list = GetMemList<int, kSize>(0);
  for (auto _ : state) {
    MemListFor<int>(list);
  }
}

static void bmStlListForHeavy(benchmark::State& state) {
  auto list = GetStlList<Heavy>(kSize, Heavy{});
  for (auto _ : state) {
    StlListFor<Heavy>(list);
  }
}

static void bmMemListForHeavy(benchmark::State& state) {
  auto list = GetMemList<Heavy, kSize>(Heavy{});
  for (auto _ : state) {
    MemListFor<Heavy>(list);
  }
}

BENCHMARK(bmStlListInsertInt);
BENCHMARK(bmMemListInsertInt);

BENCHMARK(bmStlListInsertHeavy);
BENCHMARK(bmMemListInsertHeavy);

BENCHMARK(bmStlListForInt);
BENCHMARK(bmMemListForInt);

BENCHMARK(bmStlListForHeavy);
BENCHMARK(bmMemListForHeavy);

BENCHMARK_MAIN();
