#include <list>

#include <benchmark/benchmark.h>

#include "memory_list.hpp"

namespace {

constexpr std::size_t kSize = 100'000u;

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
    result += *begin;
  }
  return result;
}

template <typename T>
T MemListFor(MemList<T, kSize>& list) {
  T result = T();
  for (auto begin = list.Begin(), end = list.End(); begin != end; ++begin) {
    result += *begin;
  }
  return result;
}

}  // namespace

static void bmStlListInsert(benchmark::State& state) {
  for (auto _ : state) {
    StlListInsert<int>(kSize, 0);
  }
}

static void bmMemListInsert(benchmark::State& state) {
  for (auto _ : state) {
    MemListInsert<int, kSize>(0);
  }
}

static void bmStlListFor(benchmark::State& state) {
  auto list = GetStlList<int>(kSize, 0);
  for (auto _ : state) {
    StlListFor<int>(list);
  }
}

static void bmMemListFor(benchmark::State& state) {
  auto list = GetMemList<int, kSize>(0);
  for (auto _ : state) {
    MemListFor<int>(list);
  }
}

BENCHMARK(bmStlListInsert);
BENCHMARK(bmMemListInsert);

BENCHMARK(bmStlListFor);
BENCHMARK(bmMemListFor);

BENCHMARK_MAIN();
