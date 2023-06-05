#include "memory_list.hpp"

#include <array>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(basic, insert) {
  MemList<int, 4> l;

  l.Insert(l.Begin(), 12);
  auto v = l.AsArray();
  ASSERT_THAT(v, ::testing::ElementsAre(12));

  l.Insert(l.Begin(), 13);
  v = l.AsArray();
  ASSERT_THAT(v, ::testing::ElementsAre(13, 12));

  auto it = l.Begin();
  ++it;

  l.Insert(it, 15);
  v = l.AsArray();
  ASSERT_THAT(v, ::testing::ElementsAre(13, 15, 12));

  l.Insert(l.End(), 16);
  v = l.AsArray();
  ASSERT_THAT(v, ::testing::ElementsAre(13, 15, 12, 16));
}

TEST(basic, push_back) {
  MemList<int, 4> l;
  l.PushBack(1);
  l.PushBack(2);
  l.PushBack(3);
  l.PushBack(4);
  auto v = l.AsArray();
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2, 3, 4));
}

TEST(basic, push_front) {
  MemList<int, 4> l;
  l.PushFront(1);
  l.PushFront(2);
  l.PushFront(3);
  l.PushFront(4);
  auto v = l.AsArray();
  ASSERT_THAT(v, ::testing::ElementsAre(4, 3, 2, 1));
}

TEST(iterators, basic) {
  MemList<int, 4> l;
  l.PushBack(1);
  l.PushBack(2);
  l.PushBack(3);
  l.PushBack(4);

  auto beg = l.Begin();
  auto end = l.End();
  std::array<int, 4> res = {1, 2, 3, 4};
  for (std::size_t i = 0; beg != end; ++beg, ++i) {
    ASSERT_EQ(*beg, res[i]);
  }
}
