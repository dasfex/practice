#pragma once

#include <vector>

// TODO In memory list и потраченное время
// 4. побенчмаркать относительно обычного листа ->
// результаты сохранить куда-нибудь, чтобы потом в статью.
// 5. попробовать нормально подряд вставлять.
// 6. delete.
// 7. уплотнение.
// 8. бенчмарки.
// 9. убрать лишний индирект попробовать.
// 10. ещё раз бенчмарки.

template <typename T, std::size_t N>
class MemList {
 private:
  struct Node {
    T* value = nullptr;  // aligned storage
    int prev;  // нужно?
    int next;
  };

 public:
  struct Iterator;

  MemList() {
    for (std::size_t i = 0; i < N; ++i) {
      SetNgb(data_[i], i - 1, i + 1);
    }
  }

  void Insert(Iterator it, const T& x) {
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
    } else if (it == Begin()) {
      data_[it.ind_].prev = node_ind;
      data_[node_ind].next = it.ind_;

      data_head_ = node_ind;
    } else if (it == End()) {
      data_[data_tail_].next = node_ind;
      data_[node_ind].prev = data_tail_;

      data_tail_ = node_ind;
    } else {
      const int left_ind = data_[it.ind_].prev;
      data_[left_ind].next = node_ind;
      data_[it.ind_].prev = node_ind;

      SetNgb(data_[node_ind], left_ind, it.ind_);
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

  void PushBack(const T& x) {
    Insert(End(), x);
  }

  void PushFront(const T& x) {
    Insert(Begin(), x);
  }

  Iterator Begin() {
    return {this, data_head_};
  }

  Iterator End() {
    return {};
  }

  struct Iterator {
    Iterator(MemList<T, N>* list = nullptr, int ind = -1)
            : list_(list), ind_(ind) {}

    Iterator& operator++() {
      if (ind_ == list_->data_tail_) {
        list_ = nullptr;
        ind_ = -1;
        return *this;
      }

      ind_ = list_->data_[ind_].next;
      return *this;
    }

    Iterator operator++() const {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    T& operator*() {
      return *list_->data_[ind_].value;
    }

    bool operator!=(const Iterator& rhs) const {
      return list_ != rhs.list_ || ind_ != rhs.ind_;
    }

    bool operator==(const Iterator& rhs) const {
      return !(*this != rhs);
    }

    friend MemList<T, N>;
   private:
    MemList<T, N>* list_ = nullptr;
    int ind_ = -1;
  };


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