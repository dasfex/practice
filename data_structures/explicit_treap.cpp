#include <utility>
#include <random>

using namespace std;

class Treap {
  public:
    Treap() : root_(nullptr) {}
  
    void Add(int x) {
        root_ = add(root_, new Node(x, std::mt19937({std::random_device{}()})()));
    }
    
    void Remove(int x) {
        root_ = remove(root_, x);
    }
  
  private:
    struct Node {
        int x;
        int y;
        Node* left;
        Node* right;
        
        Node(int x, int y, Node* left = nullptr, Node* right = nullptr) 
        : x(x), y(y), left(left), right(right) {}
    };
    
    Node* Merge(Node* left, Node* right) {
        if (left == nullptr) { return right; }
        if (right == nullptr) { return left; }
        if (left->y > right->y) {
            right->left = Merge(left, right->left);
            return right;
        } else {
            left->right = Merge(left->right, right);
            return left;
        }
    }
    
    std::pair<Node*, Node*> Split(Node* splited, int key) {
        if (splited == nullptr) { return {nullptr, nullptr}; }
        if (splited->x < key) {
            auto ab = Split(splited->right, key);
            splited->right = ab.first;
            return {splited, ab.second};
        } else {
            auto ab = Split(splited->left, key);
            splited->left = ab.second;
            return {ab.first, splited};
        }
    }
    
    Node* add(Node* tree, Node* new_node) {
        auto ab = Split(tree, new_node->x);
        return Merge(Merge(ab.first, new_node), ab.second);
    }
    
    Node* remove(Node* tree, int key) {
        auto ab = Split(tree, key + 1);
        auto a1a2 = Split(ab.first, key);
        return Merge(a1a2.first, ab.second);
    }
    
    Node* root_;
};
