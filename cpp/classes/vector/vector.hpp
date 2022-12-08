#pragma once

#include <memory>

namespace details {
#if __cplusplus >= 202002L

template <typename I>
concept iterator = requires(I i) {
  { *i };
  { ++i } -> std::same_as<I&>;
  { *i++ };
} && std::copyable<I>;

template <typename I>
concept input_iterator = iterator<I> && std::equality_comparable<I>;

#else

template <typename T>
class has_iterator_properties {
 private:

  template <typename TT,
          typename = std::void_t<
                  decltype(*std::declval<TT>()),
                  decltype(++std::declval<TT>()),
                  decltype(*std::declval<TT>()++),
                  decltype(std::declval<TT>() == std::declval<TT>()),
                  decltype(std::declval<TT>() != std::declval<TT>()),
                  std::enable_if_t<
                          std::conjunction_v<
                                  std::is_copy_constructible<TT>,
                                  std::is_object<TT>,
                                  std::is_move_constructible<TT>
                          >
                  >
          >
  >
  constexpr static std::true_type f(int);

  template <typename...>
  constexpr static std::false_type f(...);

 public:
  constexpr static bool value = decltype(f<T>(0))::value;
};

template <typename T>
constexpr inline bool has_iterator_properties_v = has_iterator_properties<T>::value;

#endif

}  // namespace details

template <
        typename T,
        typename Allocator = std::allocator<T>
>
class vector {
 public:
  using value_type = T;
  using allocator_type = Allocator;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  using iterator = value_type*;
  using const_iterator = const value_type*;
  using reverse_iterator = std::reverse_iterator<iterator>;

  constexpr iterator begin() const {
    return iterator(data_);
  }

  constexpr iterator end() const {
    return iterator(data_ + sz_);
  }

  constexpr const_iterator cbegin() const {
    return const_iterator(data_);
  }

  constexpr const_iterator cend() const {
    return const_iterator(data_ + sz_);
  }

  constexpr reverse_iterator rbegin() const {
    return reverse_iterator(end());
  }

  constexpr reverse_iterator rend() const {
    return reverse_iterator(begin());
  }

  constexpr vector() noexcept(noexcept(Allocator())) {}

  constexpr explicit vector(const Allocator& alloc) noexcept: alloc_(alloc) {}

  constexpr vector(size_type count,
                   const_reference& value,
                   const Allocator& alloc = Allocator())
          : alloc_(alloc), cp_(count), sz_(count),
            data_(alloc_.allocate(count)) {
    for (size_type i = 0; i < count; ++i) {
      push(data_, i, value);
    }
  }

  constexpr explicit vector(size_type count,
                            const Allocator& alloc = Allocator())
          : alloc_(alloc), cp_(count), sz_(count),
            data_(alloc_.allocate(count)) {
    for (size_type i = 0; i < count; ++i) {
      push(data_, i, value_type());
    }
  }

  template <typename InputIt
#if __cplusplus < 202002L
          , typename = std::enable_if_t<details::has_iterator_properties_v<InputIt>>
#endif
  >
#if __cplusplus >= 202002L
  requires details::input_iterator<InputIt>
#endif
  constexpr vector(InputIt first, InputIt last,
                   const Allocator& alloc = Allocator()) : alloc_(alloc) {
    while (first != last) {
      push_back(*first++);
    }
  }

  constexpr vector(const vector& rhs)
          : alloc_(
          std::allocator_traits<Allocator>::select_on_container_copy_construction(
                  rhs.get_allocator())), cp_(rhs.cp_), sz_(rhs.sz_),
            data_(alloc_.allocate(cp_)) {
    for (size_type i = 0; i < sz_; ++i) {
      new(data_ + i) value_type(rhs[i]);
    }
  }

  constexpr vector(const vector& rhs,
                   const Allocator& alloc)
          : alloc_(alloc), cp_(rhs.cp_), sz_(rhs.sz_),
            data_(alloc_.allocate(cp_)) {
    for (size_type i = 0; i < sz_; ++i) {
      new(data_ + i) value_type(rhs[i]);
    }
  }

  constexpr vector(vector&& rhs)
          : alloc_(std::move(rhs.alloc_)), cp_(rhs.cp_), sz_(rhs.sz_),
            data_(rhs.data_) {
    rhs.sz_ = 0;
    rhs.cp_ = 0;
    rhs.data_ = nullptr;
  }

  constexpr vector(vector&& rhs, const Allocator& alloc)
          : alloc_(alloc), cp_(rhs.cp_), sz_(rhs.sz_), data_(rhs.data_) {
    rhs.sz_ = 0;
    rhs.cp_ = 0;
    rhs.data_ = nullptr;
  }

  constexpr vector(std::initializer_list<value_type> init,
                   const Allocator& alloc = Allocator())
          : alloc_(alloc) {
    auto first = init.begin();
    auto last = init.end();
    while (first != last) {
      push_back(*first++);
    }
    shrink_to_fit();
  }

  ~vector() {
    for (size_type i = 0; i < sz_; ++i) {
      destroy(data_ + i);
    }
    alloc_.deallocate(data_, cp_);
  }

  constexpr vector& operator=(const vector& rhs) {
    if (this == &rhs) {
      return *this;
    }
    this->~vector();
    new(this) vector(rhs);
    return *this;
  }

  constexpr vector&
  operator=(vector&& rhs) noexcept(noexcept(vector(std::move(rhs)))) {
    if (this == &rhs) {
      return *this;
    }
    this->~vector();
    new(this) vector(std::move(rhs));
    return *this;
  }

  constexpr vector& operator=(std::initializer_list<value_type> it) noexcept {
    this->~vector();
    new(this) vector(it);
    return *this;
  }

  constexpr void push_back(const_reference& value) {
    if (sz_ == cp_) {
      reallocate_and_push(value);
    } else {
      push(data_, sz_, value);
      ++sz_;
    }
  }

  constexpr void push_back(value_type&& value) {
    if (sz_ == cp_) {
      reallocate_and_push(std::move(value));
    } else {
      push(data_, sz_, std::move(value));
      ++sz_;
    }
  }

  constexpr void reserve(size_type new_cp) {
    if (new_cp < cp_) {
      return;
    }
    cp_ = new_cp;
    pointer new_data = alloc_.allocate(cp_);
    if (sz_ > 0) {
      for (size_type i = 0; i < sz_; ++i) {
        new(new_data + i) value_type(std::move_if_noexcept(data_[i]));
      }
    }
    if (sz_ > 0) {
      alloc_.deallocate(data_, sz_);
    }
    data_ = new_data;
  }

  constexpr void assign(size_type count, const_reference value) {
    if (sz_ > 0) {
      alloc_.deallocate(data_, sz_);
    }
    sz_ = cp_ = count;
    pointer new_data = alloc_.allocate(cp_);
    for (size_type i = 0; i < sz_; ++i) {
      new(new_data + i) value_type(value);
    }
    data_ = new_data;
    shrink_to_fit();
  }

  template <typename InputIt
#if __cplusplus < 202002L
          , typename = std::enable_if_t<details::has_iterator_properties_v<InputIt>>
#endif
  >
#if __cplusplus >= 202002L
  requires details::input_iterator<InputIt>
#endif
  constexpr void assign(InputIt first, InputIt last) {
    while (first != last) {
      push_back(*first++);
    }
    shrink_to_fit();
  }

  constexpr void assign(std::initializer_list<value_type> init) {
    auto first = init.begin();
    auto last = init.end();
    while (first != last) {
      push_back(*first++);
    }
    shrink_to_fit();
  }

  constexpr void shrink_to_fit() {
    if (sz_ == cp_) {
      return;
    }

    cp_ = sz_;
    pointer new_data = alloc_.allocate(cp_);
    for (size_type i = 0; i < sz_; ++i) {
      new(new_data + i) value_type(std::move_if_noexcept(data_[i]));
    }
    alloc_.deallocate(data_, sz_);
    data_ = new_data;
  }

  constexpr void clear() noexcept {
    for (size_type i = 0; i < sz_; ++i) {
      destroy(data_ + i);
    }
    sz_ = 0;
  }

  template <typename... Args>
  constexpr reference emplace_back(Args&&... args) {
    if (sz_ == cp_) {
      reallocate_and_push(value_type(std::forward<Args>(args)...));
    } else {
      push(data_, sz_, value_type(std::forward<Args>(args)...));
      ++sz_;
    }
    return back();
  }

  constexpr void pop_back() {
    destroy(data_ + sz_ - 1);
    --sz_;
  }

  constexpr void resize(size_type count) {
    resize(count, value_type());
  }

  constexpr void resize(size_type count, const value_type& value) {
    if (sz_ == count) {
      return;
    }

    pointer new_data = alloc_.allocate(count);
    if (sz_ < count) {
      if (sz_ > 0) {
        for (size_type i = 0; i < sz_; ++i) {
          new(new_data + i) value_type(std::move_if_noexcept(data_[i]));
        }
        for (size_type i = sz_; i < count; ++i) {
          new(new_data + i) value_type(value);
        }
      }
    } else {
      if (count > 0) {
        for (size_type i = 0; i < count; ++i) {
          new(new_data + i) value_type(std::move_if_noexcept(data_[i]));
        }
        for (size_type i = count; i < sz_; ++i) {
          destroy(data_ + i);
        }
      } else {
        clear();
      }
    }
    alloc_.deallocate(data_, sz_);
    sz_ = cp_ = count;
    data_ = new_data;
  }

  constexpr reference operator[](size_type i) {
    return data_[i];
  }

  constexpr const_reference operator[](size_type i) const {
    return data_[i];
  }

  constexpr reference at(size_type i) {
    if (i >= size()) {
      throw std::out_of_range("at: i > size");
    }
    return data_[i];
  }

  constexpr const_reference at(size_type i) const {
    if (i >= size()) {
      throw std::out_of_range("at: i > size");
    }
    return data_[i];
  }

  constexpr reference front() {
    return data_[0];
  }

  constexpr const_reference front() const {
    return data_[0];
  }

  constexpr reference back() {
    return data_[sz_ - 1];
  }

  constexpr const_reference back() const {
    return data_[sz_ - 1];
  }

  [[nodiscard]] constexpr size_type size() const {
    return sz_;
  }

  [[nodiscard]] constexpr size_type capacity() const {
    return cp_;
  }

  constexpr pointer data() noexcept {
    return data_;
  }

  constexpr const_pointer data() const noexcept {
    return data_;
  }

  constexpr allocator_type get_allocator() const {
    return alloc_;
  }

  [[nodiscard]] constexpr bool empty() const {
    return sz_ == 0;
  }

 private:
  Allocator alloc_{};
  size_type cp_ = 0;
  size_type sz_ = 0;
  pointer data_ = nullptr;

  template <typename U, typename =
  std::enable_if_t<std::is_same_v<std::decay_t<U>, value_type>>>
  void push(pointer data, size_type i, U&& value) {
#if __cplusplus >= 202002L
    std::construct_at(data + i, std::forward<U>(value));
#else
    std::allocator_traits<Allocator>::construct(alloc_, data + i, std::forward<U>(value));
#endif
  }

  template <typename U, typename =
  std::enable_if_t<std::is_same_v<std::decay_t<U>, value_type>>>
  void reallocate_and_push(U&& value) {
    cp_ = get_new_cp();
    pointer new_data = alloc_.allocate(cp_);
    push(new_data, sz_, std::forward<U>(value));
    ++sz_;
    if (sz_ - 1 > 0) {
      for (size_type i = 0; i < sz_ - 1; ++i) {
        new(new_data + i) value_type(std::move_if_noexcept(data_[i]));
      }
    }
    alloc_.deallocate(data_, sz_ - 1);
    data_ = new_data;
  }

  size_type get_new_cp() {
    if (cp_ == 0) {
      return 1;
    }
    return cp_ * 2;
  }

  void destroy(pointer ptr) {
#if __cplusplus >= 202002L
    std::destroy_at(ptr);
#else
    std::allocator_traits<Allocator>::destroy(alloc_, ptr);
#endif
  }
};
