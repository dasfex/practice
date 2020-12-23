#ifndef DASFEX_SET
#define DASFEX_SET

#include <initializer_list>
#include <random>
#include <utility>

template<class T>
class Set{
 private:
  struct Node{
    T val;
    int prior;
    Node* parent;
    Node* left;
    Node* right;

    Node(T val, int prior, Node* parent = nullptr, Node* left = nullptr, Node* right = nullptr)
        : val(std::move(val)), prior(prior), parent(parent), left(left), right(right) {}

    ~Node() {
      delete left;
      delete right;
    }
  };

  void Update(Node* node) {
    if (node) {
      node->parent = nullptr;
      if (node->left) {
        node->left->parent = node;
      }
      if (node->right) {
        node->right->parent = node;
      }
    }
  }

  Node* MostLeft(Node* cur) const {
    while (cur && cur->left) {
      cur = cur->left;
    }
    return cur;
  }

  Node* MostRight(Node* cur) const {
    while (cur && cur->right) {
      cur = cur->right;
    }
    return cur;
  }

 public:
  class iterator{
   public:
    iterator() = default;

    iterator(const Set<T>* st, Node* node) : parent_(st), cur_(node) {}

    iterator(const iterator& other) : parent_(other.parent_), cur_(other.cur_) {}

    iterator& operator=(const iterator& rhs) {
      if (this == &rhs) {
        return *this;
      }
      this->~iterator();
      new(this) iterator(rhs);
      return *this;
    }

    const T& operator*() const {
      return cur_->val;
    }

    const T* operator->() const {
      return &(cur_->val);
    }

    iterator operator++() {
      cur_ = Next(cur_);
      return *this;
    }

    const iterator operator++(int) {
      iterator it = *this;
      cur_ = Next(cur_);
      return it;
    }

    iterator operator--() {
      cur_ = Prev(cur_);
      return *this;
    }

    const iterator operator--(int) {
      iterator it = *this;
      cur_ = Prev(cur_);
      return it;
    }

    bool operator!=(const iterator& rhs) const {
      return cur_ != rhs.cur_ && parent_ != rhs.parent_;
    }

    bool operator==(const iterator& rhs) const {
      return cur_ == rhs.cur_ && parent_ == rhs.parent_;
    }

   private:
    const Set<T>* parent_;
    Node* cur_;

    Node* Next(Node* cur) const {
      if (cur->right) {
        return parent_->MostLeft(cur->right);
      }
      if (!cur->parent) {
        return nullptr;
      }
      if (cur == cur->parent->right) {
        while (cur->parent && cur->parent->right == cur) {
          cur = cur->parent;
        }
        bool flag = false;
        if (cur->parent && cur->parent->left == cur) {
          cur = cur->parent;
          flag = true;
        }
        if (!flag && !cur->parent) {
          return nullptr;
        }
      } else {
        cur = cur->parent;
      }
      return cur;
    }

    Node* Prev(Node* cur) const {
      if (!cur) {
        return parent_->MostRight(parent_->root_);
      }
      if (cur->left) {
        return parent_->MostRight(cur->left);
      }
      if (!cur->parent) {
        return nullptr;
      }
      if (cur == cur->parent->left) {
        while (cur->parent && cur->parent->left == cur) {
          cur = cur->parent;
        }
        bool flag = false;
        if (cur->parent && cur->parent->right == cur) {
          cur = cur->parent;
          flag = true;
        }
        if (!flag && !cur->parent) {
          return nullptr;
        }
      } else {
        cur = cur->parent;
      }
      return cur;
    }
  };

  Set() = default;

  template<class ItB, class ItE>
  Set(const ItB& begin, const ItE& end) {
    for (auto it = begin; it != end; ++it) {
      insert(*it);
    }
  }

  explicit Set(std::initializer_list<T> list) {
    for (auto it = list.begin(); it != list.end(); ++it) {
      insert(*it);
    }
  }

  Set(const Set& rhs) {
    for (auto it = rhs.begin(); it != rhs.end(); ++it) {
      insert(*it);
    }
  }

  ~Set() {
    delete root_;
  }

  Set& operator=(const Set& rhs) {
    if (this == &rhs) {
      return *this;
    }
    this->~Set();
    new(this) Set(rhs);
    return *this;
  }

  [[nodiscard]] size_t size() const {
    return size_;
  }

  [[nodiscard]] bool empty() const {
    return size() == 0;
  }

  [[nodiscard]] iterator begin() const {
    return iterator(this, MostLeft(root_));
  }

  [[nodiscard]] iterator end() const {
    return iterator(this, nullptr);
  }

  void insert(const T& x) {
    if (find(x) == end()) {
      root_ = add(root_, new Node(x, gen()));
      ++size_;
    }
  }

  void erase(const T& x) {
    if (find(x) != end()) {
      root_ = remove(root_, x);
      --size_;
    }
  }

  [[nodiscard]] iterator find(const T& x) const {
    Node* cur = root_;
    while (cur) {
      if (x < cur->val) {
        cur = cur->left;
      } else if (cur->val < x) {
        cur = cur->right;
      } else {
        break;
      }
    }
    return iterator(this, cur);
  }

  [[nodiscard]] iterator lower_bound(const T& x) const {
    Node* cur = root_;
    Node* ans = nullptr;
    while (cur) {
      if (!(cur->val < x) && (!ans || cur->val < ans->val)) {
        ans = cur;
      }
      if (x < cur->val) {
        cur = cur->left;
      } else if (cur->val < x) {
        cur = cur->right;
      } else {
        ans = cur;
        break;
      }
    }
    return iterator(this, ans);
  }

 private:
  Node* root_ = nullptr;
  size_t size_ = 0;
  std::mt19937 gen = std::mt19937(std::random_device{}());

  Node* Merge(Node* left, Node* right) {
    if (!left) { return right; }
    if (!right) { return left; }
    if (left->prior > right->prior) {
      right->left = Merge(left, right->left);
      Update(right);
      return right;
    } else {
      left->right = Merge(left->right, right);
      Update(left);
      return left;
    }
  }

  std::pair<Node*, Node*> Split(Node* splited, const T& key) {
    if (!splited) { return {nullptr, nullptr}; }
    if (splited->val < key) {
      auto ab = Split(splited->right, key);
      splited->right = ab.first;
      Update(splited);
      return {splited, ab.second};
    } else {
      auto ab = Split(splited->left, key);
      splited->left = ab.second;
      Update(splited);
      return {ab.first, splited};
    }
  }

  std::pair<Node*, Node*> SplitMod(Node* splited, const T& key) {
    if (!splited) { return {nullptr, nullptr}; }
    if (key < splited->val) {
      auto ab = SplitMod(splited->left, key);
      splited->left = ab.second;
      Update(splited);
      return {ab.first, splited};
    } else {
      auto ab = SplitMod(splited->right, key);
      splited->right = ab.first;
      Update(splited);
      return {splited, ab.second};
    }
  }

  Node* add(Node* tree, Node* new_node) {
    auto ab = Split(tree, new_node->val);
    return Merge(Merge(ab.first, new_node), ab.second);
  }

  Node* remove(Node* tree, const T& key) {
    auto ab = SplitMod(tree, key);
    auto aab = Split(ab.first, key);
    delete aab.second;
    return Merge(aab.first, ab.second);
  }
};

#endif // DASFEX_SET
