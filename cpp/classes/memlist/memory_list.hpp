#pragma once

#include <vector>

// TODO In memory list и потраченное время
// 1. iterator и переделать вставку на него.
// 2. push back.
// 3. push front.
// 4. delete.
// 5. побенчмаркать относительно обычного листа ->
// результаты сохранить куда-нибудь, чтобы потом в статью.
// 6. уплотнение.
// 7. бенчмарки.
// 8. убрать лишний индирект попробовать.

template <typename T, std::size_t N>
class MemList {
 private:
  struct Node {
    T* value = nullptr;  // aligned storage
    int prev;  // нужно?
    int next;
  };

 public:
  MemList() {
    for (std::size_t i = 0; i < N; ++i) {
      SetNgb(data_[i], i - 1, i + 1);
    }
  }

  void Insert(int ind, const T& x) {  // change ind to iterator
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

    new_node.value = new T(x);
    if (size_ == 0) {
      data_head_ = data_tail_ = node_ind;

      SetNgb(data_[data_head_], -1, N);
    } else if (ind == data_head_) {
      data_[ind].prev = node_ind;
      data_[node_ind].next = ind;

      data_head_ = node_ind;
    } else {
      const int left_ind = data_[ind].prev;
      data_[left_ind].next = node_ind;
      data_[ind].prev = node_ind;

      SetNgb(data_[node_ind], left_ind, ind);
    }

    ++size_;
  }

  std::vector<T> AsArray() const {
    if (size_ == 0) {
      return {};
    }

    std::vector<T> res;
    res.reserve(size_);

    int cur_ind = data_head_;
    while (cur_ind != data_tail_) {
      res.push_back(*data_[cur_ind].value);
      cur_ind = data_[cur_ind].next;
    }
    res.push_back(*data_[data_tail_].value);

    return res;
  }

//  void PushBack(const T& x) {
//    Insert(data_tail_)
//  }

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