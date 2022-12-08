#include "vector.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <numeric>
#include <type_traits>
#include <utility>

TEST(TypeAliases, Basic) {
  using type = int;
  using vec = vector<type>;
  static_assert(std::is_same_v<vec::value_type, type>);
  static_assert(std::is_same_v<vec::allocator_type, std::allocator<type>>);
  static_assert(std::is_same_v<vec::pointer, type*>);
  static_assert(std::is_same_v<vec::const_pointer, const type*>);
  static_assert(std::is_same_v<vec::reference, type&>);
  static_assert(std::is_same_v<vec::const_reference, const type&>);
  static_assert(std::is_same_v<vec::size_type, std::size_t>);
  static_assert(std::is_same_v<vec::difference_type, std::ptrdiff_t>);
}

TEST(Constructors, DefaultConstructor) {
  vector<int> v;
  ASSERT_EQ(v.size(), 0);
  ASSERT_EQ(v.capacity(), 0);
}

TEST(Constructors, WithSizeAndInitialValue) {
  vector<int> v(5, 3);
  ASSERT_EQ(v.size(), 5);
  ASSERT_THAT(v, ::testing::ElementsAre(3, 3, 3, 3, 3));
}

TEST(Constructors, OnlySize) {
  vector<int> v(5);
  ASSERT_EQ(v.size(), 5);
  ASSERT_THAT(v, ::testing::ElementsAre(0, 0, 0, 0, 0));
}

TEST(Constructors, TwoIterators) {
  std::vector<int> vec(10);
  vector<int> v(vec.begin(), vec.end());
  for (std::size_t i = 0; i < vec.size(); ++i) {
    ASSERT_EQ(vec[i], v[i]);
  }
}

TEST(Constructors, Copy) {
  vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  vector<int> q = v;
  ASSERT_THAT(q, ::testing::ElementsAre(1, 2, 3));
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2, 3));
}

TEST(Constructors, Move) {
  vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  auto data_ptr = v.data();

  vector<int> q = std::move(v);
  ASSERT_THAT(q, ::testing::ElementsAre(1, 2, 3));
  ASSERT_EQ(q.data(), data_ptr);
}

TEST(Constructors, InitList) {
  vector<int> v = {1, 2, 3, 4, 5};
   ASSERT_EQ(v.capacity(), 5);
  ASSERT_EQ(v.size(), 5);
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2, 3, 4, 5));
}

TEST(Methods, Const) {
  const vector<int> v(5);
  ASSERT_EQ(v.size(), 5);
  ASSERT_EQ(v.capacity(), 5);
}

TEST(Methods, Empty) {
  vector<int> v;
  ASSERT_TRUE(v.empty());

  v.push_back(1);
  ASSERT_FALSE(v.empty());
}

TEST(Methods, PushBackSizes) {
  vector<int> v;
  ASSERT_EQ(v.size(), 0);
  ASSERT_EQ(v.capacity(), 0);

  v.push_back(1);
  ASSERT_EQ(v.back(), 1);
  ASSERT_EQ(v.size(), 1);
  ASSERT_EQ(v.capacity(), 1);

  v.push_back(2);
  ASSERT_EQ(v.back(), 2);
  ASSERT_EQ(v.size(), 2);
  ASSERT_EQ(v.capacity(), 2);

  v.push_back(3);
  ASSERT_EQ(v.back(), 3);
  ASSERT_EQ(v.size(), 3);
  ASSERT_EQ(v.capacity(), 4);
}

TEST(Methods, PushBackBack) {
  vector<int> v(1, 1);
  v.push_back(v.back());
  ASSERT_THAT(v, ::testing::ElementsAre(1, 1));
}

TEST(Methods, PushBackCopy) {
  int copies = 0;
  struct A {
    int& x;

    explicit A(int& r) : x(r) {}
    A(const A& a) : x(a.x) {
      ++x;
    }
    A(A&&) = delete;
  };

  vector<A> v;
  A a(copies);
  v.push_back(a);
  ASSERT_EQ(copies, 1);
}

TEST(Methods, PushBackMove) {
  int moves = 0;
  struct A {
    int& x;

    explicit A(int& r) : x(r) {}
    A(A&& a) noexcept : x(a.x) {
      ++x;
    }
  };

  vector<A> v;
  A a(moves);
  v.push_back(std::move(a));
  ASSERT_EQ(v.capacity(), 1);
  ASSERT_EQ(moves, 1);
  A b(moves);
  v.push_back(std::move(b));
  ASSERT_EQ(v.capacity(), 2);
  ASSERT_EQ(moves, 3);
}

TEST(Methods, ReserveEmpty) {
  vector<int> v;
  ASSERT_EQ(v.capacity(), 0);
  ASSERT_EQ(v.size(), 0);

  v.reserve(5);
  ASSERT_EQ(v.capacity(), 5);
  ASSERT_EQ(v.size(), 0);
}

TEST(Methods, ReserveWithData) {
  vector<int> v{1, 2, 3, 4, 5};
  ASSERT_EQ(v.capacity(), 5);
  ASSERT_EQ(v.size(), 5);

  v.reserve(10);
  ASSERT_EQ(v.capacity(), 10);
  ASSERT_EQ(v.size(), 5);
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2, 3, 4, 5));
}

TEST(Methods, ReserveDecrease) {
  vector<int> v{1, 2, 3, 4, 5};
  ASSERT_EQ(v.capacity(), 5);
  ASSERT_EQ(v.size(), 5);

  v.reserve(1);
  ASSERT_EQ(v.capacity(), 5);
  ASSERT_EQ(v.size(), 5);
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2, 3, 4, 5));
}

TEST(Methods, Assign) {
  vector<int> v;
  v.assign(5, 3);
  ASSERT_EQ(v.size(), 5);
  ASSERT_THAT(v, ::testing::ElementsAre(3, 3, 3, 3, 3));
}

TEST(Methods, AssignIterators) {
  std::vector<int> vv(3, 5);

  vector<int> v;
  v.assign(vv.begin(), vv.end());
  ASSERT_EQ(v.capacity(), 3);
  ASSERT_EQ(v.size(), 3);
  for (std::size_t i = 0; i < vv.size(); ++i) {
    ASSERT_EQ(vv[i], v[i]);
  }
}

TEST(Methods, AssignInitList) {
  vector<int> v;
  v.assign({1, 2, 3, 4, 5});
  ASSERT_EQ(v.capacity(), 5);
  ASSERT_EQ(v.size(), 5);
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2, 3, 4, 5));
}

TEST(Methods, AtCorrect) {
  vector<int> v(2, 2);
  ASSERT_EQ(v.at(0), 2);
  v.at(0) = 1;
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2));
}

TEST(Methods, AtThrow) {
  vector<int> v(2, 2);
  ASSERT_THROW(v.at(2), std::out_of_range);
}

TEST(Methods, AtConst) {
  const vector<int> v(2, 2);
  ASSERT_EQ(v.at(0), 2);
}

TEST(Methods, Back) {
  vector<int> v(1, 1);
  ASSERT_EQ(v.back(), 1);
  v.back() = 2;
  ASSERT_EQ(v.back(), 2);

  const vector<int> vv(1, 1);
  ASSERT_EQ(vv.back(), 1);
}

TEST(Methods, Front) {
  vector<int> v(2, 1);
  ASSERT_EQ(v.front(), 1);
  v.front() = 2;
  ASSERT_THAT(v, ::testing::ElementsAre(2, 1));

  const vector<int> vv(1, 1);
  ASSERT_EQ(vv.front(), 1);
}

TEST(Methods, Data) {
  static_assert(noexcept(std::declval<vector<int>>().data()));

  vector<int> v(2, 2);
  v.data()[0] = 1;
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2));

  const vector<int> vv(2, 2);
  vv.data();
}

TEST(Methods, ShrinkToFit) {
  vector<int> v{1, 2, 3, 4, 5};
  v.push_back(6);
  ASSERT_EQ(v.capacity(), 10);

  v.shrink_to_fit();
  ASSERT_EQ(v.capacity(), 6);
}

TEST(Methods, Clear) {
  vector<int> v{1, 2, 3, 4, 5};
  v.clear();
  ASSERT_THAT(v, ::testing::ElementsAre());
  ASSERT_EQ(v.size(), 0);
  ASSERT_EQ(v.capacity(), 5);
}

TEST(Methods, ClearCallDestructors) {
  static_assert(noexcept(std::declval<vector<int>>().clear()));

  int cnt = 0;
  struct A {
    int& x;
    A(int& cnt) : x(cnt) {}
    ~A() {
      ++x;
    }
  };

  vector<A> v{A(cnt), A(cnt), A(cnt)};
  v.clear();
  ASSERT_EQ(v.size(), 0);
  ASSERT_EQ(v.capacity(), 3);
  ASSERT_EQ(cnt, 6);
}

TEST(Methods, EmplaceBack) {
  struct A {
    int x;
    std::string s;
    A(int x, std::string s) : x(x), s(std::move(s)) {}
  };

  std::string s = "123";
  vector<A> v;
  v.emplace_back(1, s);
  ASSERT_EQ(v.back().x, 1);
  ASSERT_EQ(v.back().s, s);
}

TEST(Methods, PopBack) {
  vector<int> v{1, 2, 3, 4};
  v.pop_back();
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2, 3));
  ASSERT_EQ(v.size(), 3);
}

TEST(Methods, Resize) {
  vector<int> v{1, 2, 3};
  v.resize(5);
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2, 3, 0, 0));
  ASSERT_EQ(v.size(), 5);
  ASSERT_EQ(v.capacity(), 5);

  v.resize(2);
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2));
  ASSERT_EQ(v.size(), 2);
  ASSERT_EQ(v.capacity(), 2);

  v.resize(5, 3);
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2, 3, 3, 3));
}

TEST(Operators, SquareBrackets) {
  vector<int> v(2);
  auto& x = v[0];
  ASSERT_EQ(x, 0);
  x = 1;
  ASSERT_EQ(v[0], 1);
}

TEST(Operators, SquareBracketsConst) {
  const vector<int> v(2);
  auto& x = v[0];
  ASSERT_EQ(x, 0);
}

TEST(Operators, AssignCopy) {
  vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  vector<int> q;
  q = v;
  ASSERT_THAT(q, ::testing::ElementsAre(1, 2, 3));
  ASSERT_THAT(v, ::testing::ElementsAre(1, 2, 3));
}

TEST(Operators, AssignMove) {
  vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  vector<int> q;
  q = std::move(v);
  ASSERT_THAT(q, ::testing::ElementsAre(1, 2, 3));
}

TEST(Operators, AssignInitList) {
  vector<int> q;
  q = {4, 5, 6, 228};
  ASSERT_THAT(q, ::testing::ElementsAre(4, 5, 6, 228));
}

TEST(Iterators, Basic) {
  struct A {
    std::size_t x{0};
  };

  vector<A> v(1);
  auto begin = v.begin();
  auto end = v.end();
  ASSERT_NE(begin, end);

  auto& a = *begin;
  a.x = 1;
  ASSERT_EQ(a.x, 1);
  ASSERT_EQ(begin->x, 1);
  begin->x = 0;
  ASSERT_EQ(begin->x, 0);

  ++begin;
  ASSERT_EQ(begin, end);
  --begin;
  ASSERT_NE(begin, end);
  begin++;
  ASSERT_EQ(begin, end);
  begin--;
  ASSERT_NE(begin, end);

  begin = std::next(begin);
  ASSERT_EQ(begin, end);
  begin = std::prev(begin);
  ASSERT_NE(begin, end);
}

TEST(Iterators, RandomIterProperties) {
  vector<std::size_t> v(5);
  for (std::size_t i = 0; i < v.size(); ++i) {
    v[i] = i;
  }
  auto begin = v.begin();
  auto end = v.end();

  ASSERT_EQ(begin + v.size(), end);
  ASSERT_EQ(v.size() + begin, end);
  ASSERT_EQ(begin, v.begin());
  begin += v.size();
  ASSERT_EQ(begin, end);
  begin = v.begin();
  std::advance(begin, v.size() - 1);
  ASSERT_EQ(begin, end - 1);

  begin = v.begin();
  ASSERT_EQ(begin, end - v.size());
  ASSERT_EQ(end, v.end());
  end -= v.size();
  ASSERT_EQ(begin, end);
}

TEST(Iterators, RangeBasedFor) {
  vector<int> v(10);
  std::iota(v.begin(), v.end(), 0);
  int cnt = 0;
  for (int x : v) {
    ASSERT_EQ(cnt, x);
    ++cnt;
  }
}

TEST(Iterators, Const) {
  struct A {
    std::size_t x{0};
  };

  vector<A> v(1);
  auto begin = v.cbegin();
  auto end = v.cend();
  ASSERT_NE(begin, end);

  const auto& a = *begin;
  ASSERT_EQ(a.x, 0);
  ASSERT_EQ(begin->x, 0);

  ++begin;
  ASSERT_EQ(begin, end);
  --begin;
  ASSERT_NE(begin, end);
  begin++;
  ASSERT_EQ(begin, end);
  begin--;
  ASSERT_NE(begin, end);

  begin = std::next(begin);
  ASSERT_EQ(begin, end);
  begin = std::prev(begin);
  ASSERT_NE(begin, end);
}

TEST(Iterators, Reverse) {
  struct A {
    std::size_t x{0};
  };

  vector<A> v(1);
  auto begin = v.rbegin();
  auto end = v.rend();
  ASSERT_NE(begin, end);

  auto& a = *begin;
  a.x = 1;
  ASSERT_EQ(a.x, 1);
  ASSERT_EQ(begin->x, 1);
  begin->x = 0;
  ASSERT_EQ(begin->x, 0);

  ++begin;
  ASSERT_EQ(begin, end);
  --begin;
  ASSERT_NE(begin, end);
  begin++;
  ASSERT_EQ(begin, end);
  begin--;
  ASSERT_NE(begin, end);

  begin = std::next(begin);
  ASSERT_EQ(begin, end);
  begin = std::prev(begin);
  ASSERT_NE(begin, end);

  vector<int> vv{1, 2, 3, 4, 5};
  int i = 5;
  for (vector<int>::reverse_iterator
    it = vv.rbegin(), rend = vv.rend(); it != rend; ++it, --i) {
    ASSERT_EQ(*it, i);
  }
}
