#include "memory_list.hpp"

#include <array>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(basic, insert) {
  MemList<int, 4> l;

  l.Insert(l.begin(), 12);
  ASSERT_THAT(l, ::testing::ElementsAre(12));

  l.Insert(l.begin(), 13);
  ASSERT_THAT(l, ::testing::ElementsAre(13, 12));

  auto it = l.begin();
  ++it;

  l.Insert(it, 15);
  ASSERT_THAT(l, ::testing::ElementsAre(13, 15, 12));

  l.Insert(l.end(), 16);
  ASSERT_THAT(l, ::testing::ElementsAre(13, 15, 12, 16));
}

TEST(basic, push_back) {
  MemList<int, 4> l;
  l.PushBack(1);
  l.PushBack(2);
  l.PushBack(3);
  l.PushBack(4);
  ASSERT_THAT(l, ::testing::ElementsAre(1, 2, 3, 4));
}

TEST(basic, push_front) {
  MemList<int, 4> l;
  l.PushFront(1);
  l.PushFront(2);
  l.PushFront(3);
  l.PushFront(4);
  ASSERT_THAT(l, ::testing::ElementsAre(4, 3, 2, 1));
}

TEST(iterators, basic) {
  MemList<int, 4> l;
  l.PushBack(1);
  l.PushBack(2);
  l.PushBack(3);
  l.PushBack(4);

  auto beg = l.begin();
  auto end = l.end();
  std::array<int, 4> res = {1, 2, 3, 4};
  for (std::size_t i = 0; beg != end; ++beg, ++i) {
    ASSERT_EQ(*beg, res[i]);
  }
}

TEST(delete_it, front_not_full) {
  MemList<int, 4> l;
  l.PushBack(1);
  l.PushBack(2);
  l.PushBack(3);

  l.Delete(l.begin());
  ASSERT_THAT(l, ::testing::ElementsAre(2, 3));
}
