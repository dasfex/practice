#include "memory_list.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(basic, insert) {
  MemList<int, 4> l;

  l.Insert(0, 12);
  auto v = l.AsArray();
  ASSERT_THAT(v, ::testing::ElementsAre(12));

  l.Insert(0, 13);
  v = l.AsArray();
  ASSERT_THAT(v, ::testing::ElementsAre(13, 12));

  l.Insert(0, 14);
  v = l.AsArray();
  ASSERT_THAT(v, ::testing::ElementsAre(13, 14, 12));

  l.Insert(1, 14);
  v = l.AsArray();
  ASSERT_THAT(v, ::testing::ElementsAre(14, 13, 14, 12));
}
