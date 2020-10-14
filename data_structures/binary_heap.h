#include <vector>

using namespace std;

template<typename T>
class Heap {
 public:

  Heap()
      : data_(new T[10])
      , heap_size_(0)
      , capacity_(10) {}

  explicit Heap(size_t n)
      : data_(new T[n + 1])
      , heap_size_(0)
      , capacity_(n + 1) {}

  explicit Heap(const vector<T>& v)
      : data_(new T[v.size() + 1])
      , heap_size_(v.size())
      , capacity_(v.size() + 1) {
    Build(v);
  }

  void Build(const vector<T>& v) {
    heap_size_ = v.size();
    for (int i = 1; i <= heap_size_; ++i) {
      data_[i] = v[i - 1];
    }
    for (int i = heap_size_ / 2; i > 0; --i) {
      SiftDown(i);
    }
  }

  T Max() const {
    return data_[1];
  }

  void Pop() {
    data_[1] = data_[heap_size_];
    --heap_size_;
    SiftDown(1);
  }

  void Insert(const T& new_) {
    if (heap_size_ + 2 == capacity_) {
      Reserve(2);
    }
    size_t ind = ++heap_size_;
    while (ind > 1 && data_[Parent(ind)] < new_) {
      data_[ind] = data_[Parent(ind)];
      ind = Parent(ind);
    }
    data_[ind] = new_;
  }

  bool Empty() const {
    return heap_size_ == 0;
  }
 
  ~Heap() {
    delete[] data_;
  }

 private:
  T* data_;
  size_t heap_size_;
  size_t capacity_;

  inline size_t Parent(size_t i) {
    return (i >> 1);
  }

  inline size_t Left(size_t i) {
    return (i << 1);
  }

  inline size_t Right(size_t i) {
    return (i << 1) | 1;
  }

  void SiftDown(size_t ind) {
    size_t left = Left(ind);
    size_t right = Right(ind);
    size_t largest_ind;
    if (left <= heap_size_ && data_[left] > data_[ind]) {
      largest_ind = left;
    } else {
      largest_ind = ind;
    }
    if (right <= heap_size_ && data_[right] > data_[largest_ind]) {
      largest_ind = right;
    }
    if (largest_ind != ind) {
      swap(data_[largest_ind], data_[ind]);
      SiftDown(largest_ind);
    }
  }

  void Reserve(size_t mn) {
    capacity_ *= mn;
    T* tmp = new T[capacity_];
    for (int i = 1; i <= heap_size_; ++i) {
      tmp[i] = data_[i];
    }
    delete[] data_;
    data_ = tmp;
  }

};
