#include <algorithm>
#include <iostream>
#include <cassert>
#include <random>
#include <vector>

using namespace std;

class BinarySearchTree {
 public:
  BinarySearchTree();
  ~BinarySearchTree();

  bool Contains(int value) const;

  void Add(int value);
  void Erase(int value);

  int Size() const;

  std::vector<int> ToSortedArray() const;

 protected:
  struct Node {
    int value;
    Node *parent;
    Node *left;
    Node *right;

    Node(int value, Node *parent)
        : value(value), parent(parent), left(nullptr), right(nullptr) {}

    ~Node() {
      delete left;
      delete right;
    }
  };

  Node *root_;
  int size_;

  Node *Find(int value) const;

  void HangingFromFirstNodeToSecond(Node *first_node, Node *second_node);

  Node *GetLeftmostNode(Node *node) const;
  Node *GetRightmostNode(Node *node) const;

  void AppendToSortedArray(Node *node, std::vector<int> *res) const;
};

BinarySearchTree::BinarySearchTree() : root_(nullptr), size_(0) {}

BinarySearchTree::~BinarySearchTree() {
  delete root_;
}

bool BinarySearchTree::Contains(int value) const {
  return Find(value);
}

void BinarySearchTree::Add(int value) {
  if (Contains(value)) {
    return;
  }
  ++size_;
  if (root_ == nullptr) {
    root_ = new Node(value, nullptr);
    return;
  }
  Node *cur = root_;
  while (true) {
    if (cur->value > value) {
      if (cur->left == nullptr) {
        cur->left = new Node(value, cur);
        return;
      } else {
        cur = cur->left;
      }
    } else {
      if (cur->right == nullptr) {
        cur->right = new Node(value, cur);
        return;
      } else {
        cur = cur->right;
      }
    }
  }
}

void BinarySearchTree::Erase(int value) {
  Node *node = Find(value);
  if (node == nullptr) {
    return;
  }
  --size_;
  if (node->left == nullptr) {
    HangingFromFirstNodeToSecond(node, node->right);
  } else if (node->right == nullptr) {
    HangingFromFirstNodeToSecond(node, node->left);
  } else {
    Node *most_left_node = GetLeftmostNode(node->right);
    if (most_left_node->parent != node) {
      HangingFromFirstNodeToSecond(most_left_node, most_left_node->right);
      most_left_node->right = node->right;
      most_left_node->right->parent = most_left_node;
    }
    HangingFromFirstNodeToSecond(node, most_left_node);
    most_left_node->left = node->left;
    most_left_node->left->parent = most_left_node;
  }
  node->left = nullptr;
  node->right = nullptr;
  delete node;
}

std::vector<int> BinarySearchTree::ToSortedArray() const {
  std::vector<int> sorted_array;
  AppendToSortedArray(root_, &sorted_array);
  return sorted_array;
}

BinarySearchTree::Node *BinarySearchTree::Find(int value) const {
  Node *cur = root_;
  while (cur != nullptr && cur->value != value) {
    if (cur->value > value) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }
  return cur;
}

void BinarySearchTree::HangingFromFirstNodeToSecond(
    Node* first_node, Node* second_node) {
  if (first_node->parent == nullptr) {
    root_ = second_node;
  } else if (first_node->parent->left == first_node) {
    first_node->parent->left = second_node;
  } else {
    first_node->parent->right = second_node;
  }
  if (second_node != nullptr) {
    second_node->parent = first_node->parent;
  }
}

BinarySearchTree::Node *BinarySearchTree::GetLeftmostNode(Node *node) const {
  while (node->left != nullptr) {
    node = node->left;
  }
  return node;
}

BinarySearchTree::Node *BinarySearchTree::GetRightmostNode(Node *node) const {
  while (node->right != nullptr) {
    node = node->right;
  }
  return node;
}

int BinarySearchTree::Size() const {
  return size_;
}

void BinarySearchTree::AppendToSortedArray(Node *node,
                                           std::vector<int> *res) const {
  if (node == nullptr) {
    return;
  }
  AppendToSortedArray(node->left, res);
  res->push_back(node->value);
  AppendToSortedArray(node->right, res);
}

#ifndef IGNORE_MAIN
int main() {
  //Тестирование дерева
  const int kQueriesCount = 50'000;
  const std::vector<int>
      kElementsMaxValues({1, 2, 5, 13, 42, 1024, 1'000'000'000});

  mt19937_64 random_generator(2018);
  for (const int max_element_value : kElementsMaxValues) {
    vector<int> elements;
    BinarySearchTree tree;

    for (int i = 0; i < kQueriesCount; ++i) {
      int query_type = random_generator() % 3;

      switch (query_type) {
        case 0: {
          int value;
          if (elements.empty() || random_generator() % 100 < 75) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          elements.push_back(value);
          sort(elements.begin(), elements.end());
          tree.Add(value);
          break;
        }
        case 1: {
          int value;
          if (elements.empty() || random_generator() % 100 < 40) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          bool result = tree.Contains(value);
          bool expected_result =
              std::find(elements.begin(), elements.end(), value)
                  != elements.end();
          assert(result == expected_result);
          break;
        }
        case 2: {
          int value;
          if (elements.empty() || random_generator() % 100 < 25) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          auto element_it = std::find(elements.begin(), elements.end(), value);
          if (element_it != elements.end()) {
            elements.erase(element_it);
          }
          tree.Erase(value);
          break;
        }
        default: {
          break;
        }
      }

      assert(elements.size() == tree.Size());
      assert(elements == tree.ToSortedArray());
    }

    cout << "Passed: max_element_value = " << max_element_value << endl;
  }

  return 0;
}
#endif
