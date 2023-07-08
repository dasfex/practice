#include <list>

#include <benchmark/benchmark.h>

#include "memory_list.hpp"

namespace {

constexpr std::size_t kSize = 100'000u;

struct Heavy {
  char data[32768];
};

template <typename T>
std::list<T> GetStl(std::size_t n, T t = T()) {
  std::list<T> list;
  for (int i = 0; i < n; ++i) {
    list.push_back(t);
  }
  return list;
}

template <typename T, std::size_t N>
MemList<T, N> GetMem(T t = T()) {
  MemList<T, N> list;
  for (int i = 0; i < N; ++i) {
    list.PushBack(t);
  }
  return list;
}

template <typename T>
void StlInsert(std::list<T>& list, std::size_t n, T t = T()) {
  for (int i = 0; i < n; ++i) {
    list.push_back(t);
  }
}

template <typename T, std::size_t N>
void MemInsert(MemList<T, kSize>& list, T t = T()) {
  for (int i = 0; i < N; ++i) {
    list.PushBack(t);
    benchmark::ClobberMemory();
  }
}

template <typename T>
T StlFor(const std::list<T>& list) {
  T result = T();
  for (auto begin = list.begin(), end = list.end(); begin != end; ++begin) {
    benchmark::DoNotOptimize(result = *begin);
  }
  return result;
}

template <typename T>
T MemFor(MemList<T, kSize>& list) {
  T result = T();
  for (auto begin = list.begin(), end = list.end(); begin != end; ++begin) {
    benchmark::DoNotOptimize(result = *begin);
  }
  return result;
}

}  // namespace

static void bmStlCreateInt(benchmark::State& state) {
  for (auto _ : state) {
    std::list<int> list;
  }
}

static void bmMemCreateInt(benchmark::State& state) {
  for (auto _ : state) {
    MemList<int, kSize> list;
  }
}

static void bmStlCreateHeavy(benchmark::State& state) {
  for (auto _ : state) {
    std::list<Heavy> list;
  }
}

static void bmMemCreateHeavy(benchmark::State& state) {
  for (auto _ : state) {
    MemList<Heavy, kSize> list;
  }
}

static void bmStlInsertInt(benchmark::State& state) {
  std::list<int> list;
  for (auto _ : state) {
    StlInsert<int>(list, kSize, 0);
  }
}

static void bmStlInsertHeavy(benchmark::State& state) {
  std::list<Heavy> list;
  for (auto _ : state) {
    StlInsert<Heavy>(list, kSize, Heavy{});
  }
}

static void bmMemInsertInt(benchmark::State& state) {
  MemList<int, kSize> list;
  for (auto _ : state) {
    MemInsert<int, kSize>(list, 0);
  }
}

static void bmMemInsertHeavy(benchmark::State& state) {
  MemList<Heavy, kSize> list;
  for (auto _ : state) {
    MemInsert<Heavy, kSize>(list, Heavy{});
  }
}

static void bmStlForInt(benchmark::State& state) {
  auto list = GetStl<int>(kSize, 0);
  for (auto _ : state) {
    StlFor<int>(list);
  }
}

static void bmMemForInt(benchmark::State& state) {
  auto list = GetMem<int, kSize>(0);
  for (auto _ : state) {
    MemFor<int>(list);
  }
}

static void bmStlForHeavy(benchmark::State& state) {
  auto list = GetStl<Heavy>(kSize, Heavy{});
  for (auto _ : state) {
    StlFor<Heavy>(list);
  }
}

static void bmMemForHeavy(benchmark::State& state) {
  auto list = GetMem<Heavy, kSize>(Heavy{});
  for (auto _ : state) {
    MemFor<Heavy>(list);
  }
}

//BENCHMARK(bmStlCreateInt);
//BENCHMARK(bmMemCreateInt);
//BENCHMARK(bmStlCreateHeavy);
//BENCHMARK(bmMemCreateHeavy);

// BENCHMARK(bmStlInsertInt);
//BENCHMARK(bmMemInsertInt);
// BENCHMARK(bmStlInsertHeavy);
// BENCHMARK(bmMemInsertHeavy);

BENCHMARK(bmStlForInt);
BENCHMARK(bmMemForInt);
BENCHMARK(bmStlForHeavy);
BENCHMARK(bmMemForHeavy);

BENCHMARK_MAIN();
