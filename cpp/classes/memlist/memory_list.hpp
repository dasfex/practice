#pragma once

#include <type_traits>
#include <vector>

// TODO In memory list и потраченное время
// 6. delete.
// 7. уплотнение.

template <typename T, std::size_t N>
class MemList {
 private:
  struct Node {
    std::aligned_storage<sizeof(T), alignof(T)> value;
    int prev;
    int next;
  };

 public:
  struct iterator;

  using value_type = T;
  using const_iterator = const iterator;

  MemList() {

    for (std::size_t i = 0; i < N; ++i) {
      SetNgb(data_[i], i - 1, i + 1);
    }
  }

  void Insert(iterator it, const T& x) {
    if (size_ == N) {
      return;
    }

    auto& new_node = data_[empty_head_];
    const int node_ind = empty_head_;

    empty_head_ = data_[empty_head_].next;
    if (size_ + 1 < N) {
      data_[empty_head_].prev = -1;
    } else {
      empty_head_ = empty_tail_ = -1;
    }

    std::construct_at(std::launder(reinterpret_cast<T*>(&new_node.value)), x);
    if (size_ == 0) {
      data_head_ = data_tail_ = node_ind;

      SetNgb(data_[data_head_], -1, N);
    } else if (it == begin()) {
      data_[it.ind_].prev = node_ind;
      data_[node_ind].next = it.ind_;

      data_head_ = node_ind;
    } else if (it == end()) {
      data_[data_tail_].next = node_ind;
      SetNgb(data_[node_ind], data_tail_, N);

      data_tail_ = node_ind;
    } else {
      const int left_ind = data_[it.ind_].prev;
      data_[left_ind].next = node_ind;
      data_[it.ind_].prev = node_ind;

      SetNgb(data_[node_ind], left_ind, it.ind_);
    }

    ++size_;
  }

  void Delete(iterator it) {
    if (size_ == 0) {
      return;
    }

    if (it == begin()) {
      const int right_ind = data_[it.ind_].next;
      data_[right_ind].prev = -1;

      data_[empty_head_].prev = it.ind_;
      empty_head_ = it.ind_;
    } else if (it == end()) {

    } else {

    }

    std::destroy_at(std::launder(reinterpret_cast<T*>(&data_[it.ind_].value)));
  }

  void PushBack(const T& x) {
    Insert(end(), x);
  }

  void PushFront(const T& x) {
    Insert(begin(), x);
  }

  iterator begin() {
    return {this, data_head_};
  }

  const_iterator begin() const {
    return {this, data_head_};
  }

  const_iterator end() const {
    return {};
  }

  struct iterator {
    iterator(const MemList<T, N>* list = nullptr, int ind = -1)
            : list_{const_cast<MemList<T, N>*>(list)}, ind_{ind} {}

    iterator& operator++() {
      if (ind_ == list_->data_tail_) {
        list_ = nullptr;
        ind_ = -1;
        return *this;
      }

      ind_ = list_->data_[ind_].next;
      return *this;
    }

    iterator operator++() const {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    T& operator*() {
      return *std::launder(reinterpret_cast<T*>(&list_->data_[ind_].value));
    }

    const T& operator*() const {
      return *std::launder(reinterpret_cast<const T*>(&list_->data_[ind_].value));
    }

    bool operator!=(const iterator& rhs) const {
      return list_ != rhs.list_ || ind_ != rhs.ind_;
    }

    bool operator==(const iterator& rhs) const {
      return !(*this != rhs);
    }

    friend MemList<T, N>;
   private:
    MemList<T, N>* list_ = nullptr;
    int ind_ = -1;
  };

  ~MemList() {
    int cur_ind = data_head_;
    while (cur_ind != data_tail_) {
      int destroy_ind = cur_ind;
      cur_ind = data_[cur_ind].next;
      std::destroy_at(std::launder(reinterpret_cast<T*>(&data_[destroy_ind].value)));
    }
    std::destroy_at(std::launder(reinterpret_cast<T*>(&data_[data_tail_].value)));
  }


 private:
  Node data_[N];
  int data_head_ = -1;
  int data_tail_ = -1;
  int empty_head_ = 0;
  int empty_tail_ = N - 1;
  int size_ = 0;

  void SetNgb(Node& node, int prev, int next) {
    node.prev = prev;
    node.next = next;
  }
};