#include <utility>
#include <random>

class Treap {
 private:

  struct Node {
    int y;
    int val;
    int size;
    Node* left;
    Node* right;

    Node(int y, int val)
        : y(y), val(val), size(1), left(nullptr), right(nullptr) {}
  };

 public:
  Treap() : root_(nullptr) {}

  void Add(int ind, int val) {
    root_ = add(root_, ind,
                new Node(std::mt19937{std::random_device{}()}(), val));
  }

  void Remove(int ind) {
    root_ = remove(root_, ind);
  }

  void Print() const {
    print(root_);
  }

  void print(Node* t) const {
    if (t->left != nullptr) {
      print(t->left);
    }
    cout << t->val << ' ';
    if (t->right != nullptr) {
      print(t->right);
    }
  }

 private:
  void Recalc(Node* t) {
    if (t != nullptr) {
      t->size = (t->left == nullptr ? 0 : t->left->size) +
          1 + (t->right == nullptr ? 0 : t->right->size);
    }
  }

  Node* Merge(Node* left, Node* right) {
    if (left == nullptr) { return right; }
    if (right == nullptr) { return left; }
    if (left->y > right->y) {
      right->left = Merge(left, right->left);
      Recalc(right);
      return right;
    } else {
      left->right = Merge(left->right, right);
      Recalc(left);
      return left;
    }
  }

  std::pair<Node*, Node*> Split(Node* splited, int key) {
    if (splited == nullptr) { return {nullptr, nullptr}; }
    int cur_ind = 1 + (splited->left != nullptr ? splited->left->size : 0);
    if (cur_ind <= key) {
      auto ab = Split(splited->right, key - cur_ind);
      splited->right = ab.first;
      Recalc(splited);
      return {splited, ab.second};
    } else {
      auto ab = Split(splited->left, key);
      splited->left = ab.second;
      Recalc(splited);
      return {ab.first, splited};
    }
  }

  Node* add(Node* tree, int ind, Node* new_node) {
    auto ab = Split(tree, ind - 1);
    return Merge(Merge(ab.first, new_node), ab.second);
  }

  Node* remove(Node* tree, int key) {
    auto ab = Split(tree, key);
    auto a1a2 = Split(ab.first, key - 1);
    return Merge(a1a2.first, ab.second);
  }

  Node* root_;
};
