#ifndef DASFEX_HASH_MAP
#define DASFEX_HASH_MAP

#include <algorithm>
#include <list>
#include <stdexcept>
#include <utility>
#include <vector>

// use type-deduction(C++17)
template<class Key, class Val, class Hasher = std::hash<Key>>
class HashMap{
 public:
  using iterator = typename std::list<std::pair<const Key, Val>>::iterator;
  using const_iterator = typename std::list<std::pair<const Key, Val>>::const_iterator;

  HashMap(Hasher hasher = Hasher()) : hasher_(hasher) {
  }

  template<class Iterator>
  HashMap(Iterator begin, Iterator end, Hasher hasher = Hasher()) : hasher_(hasher) {
    for (auto it = begin; it != end; ++it) {
      insert(*it);
    }
  }

  HashMap(std::initializer_list<std::pair<Key, Val>> list, Hasher hasher = Hasher()) : hasher_(
      hasher) {
    for (const auto& pair : list) {
      insert(pair);
    }
  }

  HashMap(const HashMap<Key, Val, Hasher>& rhs)
      : hasher_(rhs.hasher_), max_load_factor_(rhs.max_load_factor_) {
    for (const auto& x : rhs) {
      std::pair<Key, Val> new_x = std::make_pair(x.first, x.second);
      insert(new_x);
    }
  }

  HashMap& operator=(const HashMap& rhs) {
    if (this == &rhs) {
      return *this;
    }
    this->~HashMap();
    new(this) HashMap(rhs);
    return *this;
  }

  size_t size() const {
    return list_.size();
  }

  bool empty() const {
    return size() == 0;
  }

  Hasher hash_function() const {
    return hasher_;
  }

  double load_factor() const {
    return static_cast<double>(list_.size()) / buckets_.size();
  }

  void clear() {
    std::fill(buckets_.begin(), buckets_.end(), std::make_pair(list_.end(), 0));
    list_.clear();
  }

  void max_load_factor(double new_mlf) {
    max_load_factor_ = new_mlf;
  }

  const Val& at(const Key& key) const {
    auto pos = find(key);
    if (pos == list_.end()) {
      throw std::out_of_range("There is no such pair in map.");
    }
    return pos->second;
  }

  Val& operator[](const Key& key) {
    auto pos = find(key);
    if (pos == list_.end()) {
      insert(std::make_pair(key, Val()));
      pos = find(key);
    }
    return pos->second;
  }

  void insert(const std::pair<Key, Val>& new_pair) {
    auto is_in = find(new_pair.first);
    if (is_in != list_.end()) {
      return;
    }
    size_t hash = get_hash(new_pair.first);
    iterator pos = buckets_[hash].first;
    list_.insert(pos, new_pair);
    --pos;
    buckets_[hash] = std::make_pair(pos, buckets_[hash].second + 1);
    if (load_factor() > max_load_factor_) {
      reallocate(buckets_.size() * 2);
    }
  }

  void erase(const Key& key) {
    auto pos = find(key);
    if (pos == list_.end()) {
      return;
    }
    size_t hash = get_hash(key);
    --buckets_[hash].second;
    if (buckets_[hash].second == 0) {
      buckets_[hash].first = list_.end();
    } else if (pos == buckets_[hash].first) {
      ++buckets_[hash].first;
    }
    list_.erase(pos);
  }

  iterator find(const Key& key) {
    size_t hash = get_hash(key);
    size_t cur = 0;
    for (auto[it, size] = buckets_[hash]; cur < size; ++it, ++cur) {
      if (it->first == key) {
        return it;
      }
    }
    return list_.end();
  }

  const_iterator find(const Key& key) const {
    size_t hash = get_hash(key);
    size_t cur = 0;
    for (auto[it, size] = buckets_[hash]; cur < size; ++it, ++cur) {
      if (it->first == key) {
        return const_iterator(it);
      }
    }
    return list_.cend();
  }

  iterator begin() {
    return list_.begin();
  }

  iterator end() {
    return list_.end();
  }

  const_iterator begin() const {
    return list_.cbegin();
  }

  const_iterator end() const {
    return list_.cend();
  }

 private:
  std::list<std::pair<const Key, Val>> list_{};
  std::vector<std::pair<iterator, size_t>> buckets_ =
      std::vector<std::pair<iterator, size_t>>(
          1, std::make_pair(list_.end(), 0));
  Hasher hasher_;
  double max_load_factor_ = 1;

  // non const cause hasher_ can change
  size_t get_hash(const Key& key) const {
    return hasher_(key) % buckets_.size();
  }

  void reallocate(size_t new_size) {
    std::list copy = list_;
    list_.clear();
    buckets_.clear();
    buckets_.resize(new_size, std::make_pair(list_.end(), 0));
    for (auto& pair : copy) {
      insert(pair);
    }
  }
};

#endif // DASFEX_HASH_MAP
