#include <iostream>
#include <iterator>
#include <vector>
#include <functional>
#include <exception>
#include <utility>
#include <cassert>
#include <string>
#include <random>
#include <list>
#include <cstdint>

template<typename T>
class BiDirectionalList {
 protected:
  class Node;

 public:
  class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
   public:
    T& operator*() const;
    T* operator->() const;

    Iterator& operator++();
    const Iterator operator++(int);

    Iterator& operator--();
    const Iterator operator--(int);

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;

   private:
    friend class BiDirectionalList;

    const BiDirectionalList* const list_;
    Node* node_;

    Iterator(const BiDirectionalList* const list, Node* node)
        : list_(list), node_(node) {}
  };

  class ConstIterator :
      public std::iterator<std::bidirectional_iterator_tag, T> {
   public:
    const T& operator*() const;
    const T* operator->() const;

    ConstIterator& operator++();
    const ConstIterator operator++(int);

    ConstIterator& operator--();
    const ConstIterator operator--(int);

    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;

   private:
    friend class BiDirectionalList;

    const BiDirectionalList* const list_;
    const Node* node_;

    ConstIterator(const BiDirectionalList* const list, Node* node)
        : list_(list)
        , node_(node) {}
  };

  BiDirectionalList()
      : first_(nullptr)
      , last_(nullptr) {}

  ~BiDirectionalList() { Clear(); }

  bool IsEmpty() const;

  void Clear();

  Iterator begin();
  Iterator end();

  ConstIterator cbegin() const;
  ConstIterator cend() const;

  std::vector<T> AsArray() const;

  void InsertBefore(Iterator position, const T& value);
  void InsertBefore(Iterator position, T&& value);

  void InsertAfter(Iterator position, const T& value);
  void InsertAfter(Iterator position, T&& value);

  void PushBack(const T& value);
  void PushBack(T&& value);

  void PushFront(const T& value);
  void PushFront(T&& value);

  void Erase(Iterator position);

  void PopFront();
  void PopBack();

  Iterator Find(const T& value);
  ConstIterator Find(const T& value) const;

  Iterator Find(std::function<bool(const T&)> predicate);
  ConstIterator Find(std::function<bool(const T&)> predicate) const;

 protected:
  struct Node {

    // Считаю, что в данном случае реализацию методов структуры
    // можно не выносить, т.к. требуется просто добавить списки
    // инициализации, и никаких изменений больше происходить не
    // будет(проект небольшой).

    explicit Node(const T& value)
        : value_(value)
        , previous_node_(nullptr)
        , next_node_(nullptr) {}

    explicit Node(T&& value)
        : value_(value)
        , previous_node_(nullptr)
        , next_node_(nullptr) {
      value = T();
    }

    T value_;
    Node* previous_node_;
    Node* next_node_;
  };

  Node* first_;
  Node* last_;

  void InsertBefore(Node* existing_node, Node* new_node);
  void InsertAfter(Node* existing_node, Node* new_node);
  void Erase(Node* node);
};

template<typename T>
T& BiDirectionalList<T>::Iterator::operator*() const {
  return node_->value_;
}

template<typename T>
T* BiDirectionalList<T>::Iterator::operator->() const {
  return &(node_->value_);
}

template<typename T>
typename BiDirectionalList<T>::Iterator&
BiDirectionalList<T>::Iterator::operator++() {
  if (node_ == nullptr) {
    throw std::runtime_error("Can't increase nullptr.");
  } else {
    node_ = node_->next_node_;
    return *this;
  }
}

template<typename T>
const typename BiDirectionalList<T>::Iterator
BiDirectionalList<T>::Iterator::operator++(int) {
  if (node_ == nullptr) {
    throw std::runtime_error("Can't increase nullptr.");
  }
  Iterator ret_iter = *this;
  node_ = node_->next_node_;
  return ret_iter;
}

template<typename T>
typename BiDirectionalList<T>::Iterator&
BiDirectionalList<T>::Iterator::operator--() {
  if (node_ == list_->first_) {
    throw std::runtime_error("Can't reduce iterator.");
  } else if (node_ == nullptr) {
    node_ = list_->last_;
  } else {
    node_ = node_->previous_node_;
  }
  return *this;
}

template<typename T>
const typename BiDirectionalList<T>::Iterator
BiDirectionalList<T>::Iterator::operator--(int) {
  if (node_ == list_->first_) {
    throw std::runtime_error("Can't reduce iterator.");
  }
  Iterator ret_iter = *this;
  if (node_ == nullptr) {
    node_ = list_->last_;
  } else {
    node_ = node_->previous_node_;
  }
  return ret_iter;
}

template<typename T>
bool BiDirectionalList<T>::Iterator::operator==(
    const BiDirectionalList::Iterator& other) const {
  return node_ == other.node_;
}

template<typename T>
bool BiDirectionalList<T>::Iterator::operator!=(
    const BiDirectionalList::Iterator& other) const {
  return !(*this == other);
}

template<typename T>
const T& BiDirectionalList<T>::ConstIterator::operator*() const {
  return node_->value_;
}

template<typename T>
const T* BiDirectionalList<T>::ConstIterator::operator->() const {
  return &(node_->value_);
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator&
BiDirectionalList<T>::ConstIterator::operator++() {
  if (node_ == nullptr) {
    throw std::runtime_error("Can't increase nullptr.");
  } else {
    node_ = node_->next_node_;
    return *this;
  }
}

template<typename T>
const typename BiDirectionalList<T>::ConstIterator
BiDirectionalList<T>::ConstIterator::operator++(int) {
  if (node_ == nullptr) {
    throw std::runtime_error("Can't increase nullptr.");
  }
  ConstIterator ret_iter = *this;
  node_ = node_->next_node_;
  return ret_iter;
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator&
BiDirectionalList<T>::ConstIterator::operator--() {
  if (node_ == list_->first_) {
    throw std::runtime_error("Can't reduce iterator.");
  } else if (node_ == nullptr) {
    node_ = list_->last_;
  } else {
    node_ = node_->previous_node_;
  }
  return *this;
}

template<typename T>
const typename BiDirectionalList<T>::ConstIterator
BiDirectionalList<T>::ConstIterator::operator--(int) {
  if (node_ == list_->first_) {
    throw std::runtime_error("Can't reduce iterator.");
  }
  ConstIterator ret_iter = *this;
  if (node_ == nullptr) {
    node_ = list_->last_;
  } else {
    node_ = node_->previous_node_;
  }
  return ret_iter;
}

template<typename T>
bool BiDirectionalList<T>::ConstIterator::operator==(
    const BiDirectionalList::ConstIterator& other) const {
  return node_ == other.node_;
}

template<typename T>
bool BiDirectionalList<T>::ConstIterator::operator!=(
    const BiDirectionalList::ConstIterator& other) const {
  return !(*this == other);
}

template<typename T>
bool BiDirectionalList<T>::IsEmpty() const {
  return first_ == last_ && first_ == nullptr;
}

template<typename T>
void BiDirectionalList<T>::Clear() {
  while (!IsEmpty()) {
    Erase(last_);
  }
}

template<typename T>
typename BiDirectionalList<T>::Iterator BiDirectionalList<T>::begin() {
  return BiDirectionalList::Iterator(this, first_);
}

template<typename T>
typename BiDirectionalList<T>::Iterator BiDirectionalList<T>::end() {
  return BiDirectionalList::Iterator(this, nullptr);
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator
BiDirectionalList<T>::cbegin() const {
  return BiDirectionalList::ConstIterator(this, first_);
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator
BiDirectionalList<T>::cend() const {
  return BiDirectionalList::ConstIterator(this, nullptr);
}

template<typename T>
std::vector<T> BiDirectionalList<T>::AsArray() const {
  std::vector<T> return_vector;
  for (auto it = cbegin(); it != cend(); ++it) {
    return_vector.push_back(*it);
  }
  return return_vector;
}

template<typename T>
void BiDirectionalList<T>::InsertBefore(
    BiDirectionalList::Iterator position, const T& value) {
  Node* new_node = new Node(value);
  InsertBefore(position.node_, new_node);
}

template<typename T>
void BiDirectionalList<T>::InsertBefore(
    BiDirectionalList::Iterator position, T&& value) {
  Node* new_node = new Node(std::move(value));
  InsertBefore(position.node_, new_node);
}

template<typename T>
void BiDirectionalList<T>::InsertAfter(
    BiDirectionalList::Iterator position, const T& value) {
  Node* new_node = new Node(value);
  InsertAfter(position.node_, new_node);
}

template<typename T>
void BiDirectionalList<T>::InsertAfter(
    BiDirectionalList::Iterator position, T&& value) {
  Node* new_node = new Node(std::move(value));
  InsertAfter(position.node_, new_node);
}

template<typename T>
void BiDirectionalList<T>::PushBack(const T& value) {
  Node* new_node = new Node(value);
  InsertAfter(last_, new_node);
}

template<typename T>
void BiDirectionalList<T>::PushBack(T&& value) {
  Node* new_node = new Node(std::move(value));
  InsertAfter(last_, new_node);
}

template<typename T>
void BiDirectionalList<T>::PushFront(const T& value) {
  Node* new_node = new Node(value);
  InsertBefore(first_, new_node);
}

template<typename T>
void BiDirectionalList<T>::PushFront(T&& value) {
  Node* new_node = new Node(std::move(value));
  InsertBefore(first_, new_node);
}

template<typename T>
void BiDirectionalList<T>::Erase(BiDirectionalList::Iterator position) {
  if (IsEmpty()) {
    throw std::runtime_error("Can't delete from empty list.");
  }
  if (position == end()) {
    throw std::runtime_error("Can't delete end().");
  }
  Erase(position.node_);
}

template<typename T>
void BiDirectionalList<T>::PopFront() {
  if (IsEmpty()) {
    throw std::runtime_error("Can't delete empty list.");
  }
  Erase(begin().node_);
}

template<typename T>
void BiDirectionalList<T>::PopBack() {
  if (IsEmpty()) {
    throw std::runtime_error("Can't delete empty list.");
  }
  Erase((--end()).node_);
}

template<typename T>
typename BiDirectionalList<T>::Iterator
BiDirectionalList<T>::Find(const T& value) {
  for (Iterator it = begin(); it != end(); ++it) {
    if (*it == value) {
      return it;
    }
  }
  return end();
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator
BiDirectionalList<T>::Find(const T& value) const {
  for (ConstIterator it = cbegin(); it != cend(); ++it) {
    if (*it == value) {
      return it;
    }
  }
  std::cerr << "!!!" << std::endl;
  return end();
}

template<typename T>
typename BiDirectionalList<T>::Iterator
BiDirectionalList<T>::Find(std::function<bool(const T&)> predicate) {
  for (Iterator it = begin(); it != end(); ++it) {
    if (predicate(*it)) {
      return it;
    }
  }
  return end();
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator
BiDirectionalList<T>::Find(std::function<bool(const T&)> predicate) const {
  for (ConstIterator it = cbegin(); it != cend(); ++it) {
    if (predicate(*it)) {
      return it;
    }
  }
  return end();
}

template<typename T>
void BiDirectionalList<T>::InsertBefore(
    BiDirectionalList::Node* existing_node,
    BiDirectionalList::Node* new_node) {
  if (first_ == nullptr) {
    first_ = last_ = new_node;
  } else if (existing_node == first_) {
    new_node->next_node_ = existing_node;
    existing_node->previous_node_ = new_node;
    first_ = new_node;
  } else {
    Node* ex_previous_node = existing_node->previous_node_;
    existing_node->previous_node_ = new_node;
    new_node->previous_node_ = ex_previous_node;
    ex_previous_node->next_node_ = new_node;
    new_node->next_node_ = existing_node;
  }
}
template<typename T>
void BiDirectionalList<T>::InsertAfter(
    BiDirectionalList::Node* existing_node,
    BiDirectionalList::Node* new_node) {
  if (IsEmpty()) {
    first_ = last_ = new_node;
  } else if (existing_node == last_) {
    existing_node->next_node_ = new_node;
    new_node->previous_node_ = existing_node;
    last_ = new_node;
  } else {
    Node* ex_next = existing_node->next_node_;
    existing_node->next_node_ = new_node;
    new_node->previous_node_ = existing_node;
    ex_next->previous_node_ = new_node;
    new_node->next_node_ = ex_next;
  }
}

template<typename T>
void BiDirectionalList<T>::Erase(BiDirectionalList::Node* node) {
  if (node->next_node_ == nullptr && node->previous_node_ == nullptr) {
    delete node;
    first_ = last_ = nullptr;
  } else if (node->next_node_ == nullptr) {
    Node* previous = node->previous_node_;
    delete node;
    last_ = previous;
    last_->next_node_ = nullptr;
  } else if (node->previous_node_ == nullptr) {
    Node* next = node->next_node_;
    delete node;
    first_ = next;
    first_->previous_node_ = nullptr;
  } else {
    Node* previous = node->previous_node_;
    Node* next = node->next_node_;
    previous->next_node_ = next;
    next->previous_node_ = previous;
    delete node;
  }
}

//#define SKIP_BASIC
//#define SKIP_POP
//#define SKIP_FIND
//#define SKIP_HARD_INSERT
//#define SKIP_ERASE
//#define SKIP_FEW_ITERATORS
//#define SKIP_CONST_ITERATORS

int main() {
#ifndef SKIP_BASIC
  {
    BiDirectionalList<int> list;

    assert(list.IsEmpty());
    list.PushBack(5);
    assert(!list.IsEmpty());

    list.PushBack(3);
    list.PushBack(12);
    list.PushBack(18);
    std::vector<int> check = list.AsArray();
    std::vector<int> right_answer = {5, 3, 12, 18};
    assert(check == right_answer);

    list.Clear();
    assert(list.IsEmpty());

    list.PushFront(5);
    assert(!list.IsEmpty());
    list.PushFront(18);
    list.PushFront(32);
    check = list.AsArray();
    right_answer = {32, 18, 5};
    assert(check == right_answer);

    list.Clear();

    list.PushFront(1);
    list.PushBack(2);
    list.PushFront(3);
    list.PushBack(4);
    check = list.AsArray();
    right_answer = {3, 1, 2, 4};
    assert(check == right_answer);

    list.Clear();

    int insert = 5;
    list.PushBack(insert);
    list.PushBack(insert);
    list.PushFront(insert);
    check = list.AsArray();
    right_answer = {5, 5, 5};
    assert(check == right_answer);
  }
  std::cout << "[PASS] BASIC :)" << std::endl;
#else
  std::cout << "[SKIPPED] BASIC :(" << std::endl;
#endif // SKIP_BASIC

#ifndef SKIP_POP
  {
    BiDirectionalList<int> list;
    list.PushFront(1);
    list.PushBack(2);
    list.PushBack(3);
    list.PushFront(0);

    list.PopBack();
    std::vector<int> check = list.AsArray();
    std::vector<int> right_answer = {0, 1, 2};
    assert(check == right_answer);

    list.PopFront();
    check = list.AsArray();
    right_answer = {1, 2};
    assert(check == right_answer);

    list.PopFront();
    list.PopFront();

    try {
      list.PopBack();
      assert(0 == 1); // never
    } catch (std::runtime_error& error) {
      assert(static_cast<std::string>(error.what())
                 == "Can't delete empty list.");
    }
    assert(list.IsEmpty());
  }
  std::cout << "[PASS] POP :)" << std::endl;
#else
  std::cout << "[SKIPPED] POP" << std::endl;
#endif

#ifndef SKIP_FIND
  {
    BiDirectionalList<int> list;
    list.PushBack(18);
    list.PushBack(137);
    list.PushFront(1007);
    BiDirectionalList<int>::Iterator result_1 = list.Find(137);
    assert(*result_1 == 137);

    // Чтобы не менять интерфейс класса, не перегружал = для итераторов.
    // Потому создаю новый.
    BiDirectionalList<int>::Iterator result_2 = list.Find(138);
    assert(result_2 == list.end());

    auto FirstFunction = [](const int& value) {
      return value == 1007;
    };
    BiDirectionalList<int>::Iterator result_3 = list.Find(FirstFunction);
    assert(*result_3 == 1007);

    auto SecondFunction = [](const int& value) {
      return value % 5 == 0;
    };
    BiDirectionalList<int>::Iterator result_4 = list.Find(SecondFunction);
    assert(result_4 == list.end());

    BiDirectionalList<std::string> string_list;
    string_list.PushFront("abacaba");
    string_list.PushFront("q");
    string_list.PushBack("aaa");

    BiDirectionalList<std::string>::Iterator result_5 = string_list.Find("q");
    assert(*result_5 == "q");

    auto StringSizeChecker = [](const std::string& str) {
      return str.size() == 100;
    };
    BiDirectionalList<std::string>::Iterator
        result_6 = string_list.Find(StringSizeChecker);
    assert(result_6 == string_list.end());
  }
  std::cout << "[PASS] FIND :)" << std::endl;
#else
  std::cout << "[SKIPPED] FIND :(" << std::endl;
#endif // SKIP_FIND

#ifndef SKIP_HARD_INSERT
  {
    BiDirectionalList<int> list;
    list.PushFront(3);
    list.PushFront(1);
    BiDirectionalList<int>::Iterator position = list.Find(3);
    list.InsertBefore(position, 2);
    std::vector<int> check = list.AsArray();
    std::vector<int> right_answer = {1, 2, 3};
    assert(check == right_answer);

    list.InsertAfter(position, 4);
    right_answer = {1, 2, 3, 4};
    check = list.AsArray();
    assert(check == right_answer);

    int q = 0;
    list.InsertBefore(list.begin(), q);
    right_answer = {0, 1, 2, 3, 4};
    check = list.AsArray();
    assert(check == right_answer);

    q = 5;
    list.InsertAfter(--list.end(), q);
    right_answer = {0, 1, 2, 3, 4, 5};
    check = list.AsArray();
    assert(check == right_answer);
  }
  std::cout << "[PASS] HARD INSERT :)" << std::endl;
#else
  std::cout << "[SKIPPED] HARD INSERT :(" << std::endl;
#endif // SKIP_HARD_INSERT

#ifndef SKIP_ERASE
  {
    BiDirectionalList<int> list;
    list.PushFront(12);
    list.PushBack(135);

    list.Erase(list.Find(12));
    std::vector<int> check = list.AsArray();
    std::vector<int> right_answer = {135};
    assert(check == right_answer);

    try {
      list.Erase(list.end());
    } catch (std::runtime_error& error) {
      assert(static_cast<std::string>(error.what())
                 == "Can't delete end().");
    }

    list.Erase(list.Find(135));
    assert(list.IsEmpty());

    try {
      list.Erase(list.begin());
    } catch (std::runtime_error& error) {
      assert(static_cast<std::string>(error.what())
                 == "Can't delete from empty list.");
    }

  }
  std::cout << "[PASS] ERASE :)" << std::endl;
#else
  std::cout << "[SKIPPED] ERASE :(" << std::endl;
#endif

// Подтверждением тому, что итераторы работаю, служит тот факт,
// что работает всё остальное. Протестируем только перегрузки
// декремента, т.к. их мы не использовали.

#ifndef SKIP_FEW_ITERATORS
  {
    BiDirectionalList<int> list;
    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);
    list.PushBack(4);
    list.PushBack(5);

    BiDirectionalList<int>::Iterator it_1 = list.end();
    std::vector<int> check;
    for (--it_1; it_1 != list.begin(); --it_1) {
      check.push_back(*it_1);
    }
    std::vector<int> right_answer = {5, 4, 3, 2};
    assert(check == right_answer);

    BiDirectionalList<int>::Iterator it_2 = list.end();

    BiDirectionalList<int>::Iterator it_3 = --it_2;
    assert(it_3 == --list.end());

    ++it_2;

    BiDirectionalList<int>::Iterator it_4 = it_2--;
    assert(it_4 == list.end());
    assert(*it_2 == 5);

    BiDirectionalList<int>::Iterator it_5 = list.begin();

    BiDirectionalList<int>::Iterator it_6 = ++it_5;
    assert(it_6 == ++list.begin());

    BiDirectionalList<int>::Iterator it_7 = list.begin();

    BiDirectionalList<int>::Iterator it_8 = it_7++;
    assert(it_8 == list.begin());
    assert(*it_7 == 2);
  }
  std::cout << "[PASS] FEW ITERATORS :)" << std::endl;
#else
  std::cout << "[SKIPPED] FEW ITERATORS :(" << std::endl;
#endif

#ifndef SKIP_CONST_ITERATORS
  {
    BiDirectionalList<int> list;

    list.PushBack(5);
    BiDirectionalList<int>::Iterator iterator = list.begin();
    assert(*iterator == 5);
    *iterator = 6;
    assert(*iterator == 6);

    BiDirectionalList<int>::ConstIterator const_iterator = list.cbegin();
    assert(*const_iterator == 6);

    // Следующая строка не скомпилируется.
    //*const_iterator = 6;
  }
  {
    BiDirectionalList<int> list;
    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);
    list.PushBack(4);
    list.PushBack(5);

    BiDirectionalList<int>::ConstIterator it_1 = list.cend();
    std::vector<int> check;
    for (--it_1; it_1 != list.cbegin(); --it_1) {
      check.push_back(*it_1);
    }
    std::vector<int> right_answer = {5, 4, 3, 2};
    assert(check == right_answer);

    BiDirectionalList<int>::ConstIterator it_2 = list.cend();

    BiDirectionalList<int>::ConstIterator it_3 = --it_2;
    assert(it_3 == --list.cend());

    ++it_2;

    BiDirectionalList<int>::ConstIterator it_4 = it_2--;
    assert(it_4 == list.cend());
    assert(*it_2 == 5);

    BiDirectionalList<int>::ConstIterator it_5 = list.cbegin();

    BiDirectionalList<int>::ConstIterator it_6 = ++it_5;
    assert(it_6 == ++list.cbegin());

    BiDirectionalList<int>::ConstIterator it_7 = list.cbegin();

    BiDirectionalList<int>::ConstIterator it_8 = it_7++;
    assert(it_8 == list.cbegin());
    assert(*it_7 == 2);
  }
  std::cout << "[PASS] CONST ITERATORS :)" << std::endl;
#else
  std::cout << "[SKIPPED] CONST ITERATORS :(" << std::endl;
#endif // SKIP_CONST_ITERATORS

#ifndef SKIP_STRUCT
  {
    struct MyStruct {
      int first;
      int second;

      bool operator==(const MyStruct& other) {
        return first == other.first && second == other.second;
      }
    };
    BiDirectionalList<MyStruct> list;
    list.PushBack({1, 1});
    list.PushBack({1, 2});
    list.PushFront({0, 0});

    std::vector<MyStruct> check = list.AsArray();
    std::vector<MyStruct> right_answer = {{0, 0},
                                          {1, 1},
                                          {1, 2}};
    for (int i = 0; i < check.size(); ++i) {
      assert(check[i] == right_answer[i]);
    }
  }
  std::cout << "[PASS] STRUCT :)" << std::endl;
#else
  std::cout << "[SKIPPED] STRUCT :(" << std::endl;
#endif // SKIP_STRUCT

#ifndef SKIP_RANDOM_TESTS
  {
    std::mt19937 generator(2019);
    std::list<int> stl_list;
    BiDirectionalList<int> my_list;

    uint32_t test_count = generator() % 1'000'000 + 1;
    assert(test_count > 0);
    for (uint32_t i = 0; i < test_count; ++i) {
      int type = generator() % 6;
      switch (type) {
        case (0) : {
          int value = generator() % 100'000;
          stl_list.push_back(value);
          my_list.PushBack(value);
          break;
        }
        case (1) : {
          int value = generator() % 100'000;
          stl_list.push_front(value);
          my_list.PushFront(value);
          break;
        }
        case (2) : {
          if (!my_list.IsEmpty() && !stl_list.empty()) {
            stl_list.pop_back();
            my_list.PopBack();
          }
          break;
        }
        case (3) : {
          if (!my_list.IsEmpty() && !stl_list.empty()) {
            stl_list.pop_front();
            my_list.PopFront();
          }
          break;
        }
        case (4) : {
          BiDirectionalList<int>::Iterator my_it = my_list.begin();
          std::list<int>::iterator stl_it = stl_list.begin();
          for (; my_it != my_list.end() && stl_it != stl_list.end();
                 ++my_it, ++stl_it) {
            assert(*my_it == *stl_it);
          }
        }
        case (5) : {
          assert(my_list.IsEmpty() == stl_list.empty());
          break;
        }
      }
    }
  }
  std::cout << "[PASS] RANDOM TESTS :)" << std::endl;
#else
  std::cout << "[SKIPPED] RANDOM TESTS :(" << std::endl;
#endif // SKIP_RANDOM_TESTS

  return 0;
}

//////////////////////////////////////////////////////////
/////////////////////// Second version ///////////////////
//////////////////////////////////////////////////////////

#pragma once

template <typename T>
class List {
private:
    struct Node {
        T val;
        Node* prev = nullptr;
        Node* next = nullptr;

        Node(const T& val) : val(val) {
        }

        Node(T&& val) : val(std::move(val)) {
        }

        Node(const Node* node) {
            if (node == nullptr) {
                return;
            }
            val = node->val;
        }
    };

public:
    class Iterator {
    public:
        Iterator(Node* cur, Node* end) : cur_(cur), end_(end) {
        }

        Iterator& operator++() {
            cur_ = cur_->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator cur(cur_, end_);
            ++*this;
            return cur;
        }

        Iterator& operator--() {
            if (!cur_) {
                cur_ = end_;
            } else {
                cur_ = cur_->prev;
            }
            return *this;
        }

        Iterator operator--(int) {
            Iterator cur(cur_, end_);
            --*this;
            return cur;
        }

        T& operator*() const {
            return cur_->val;
        }

        T* operator->() const {
            return &(cur_->val);
        }

        bool operator==(const Iterator& rhs) const {
            return cur_ == rhs.cur_;
        }

        bool operator!=(const Iterator& rhs) const {
            return cur_ != rhs.cur_;
        }

    private:
        Node* cur_;
        Node* end_;  // for -- when nullptr
    };

    List() : begin_(nullptr), end_(nullptr), size_(0) {
    }

    List(const List& list) : begin_(nullptr), end_(nullptr), size_(list.size_) {
        if (!list.begin_) {
            return;
        }
        begin_ = end_ = new Node(list.begin_->val);
        Node* cur_cp = list.begin_;
        Node* cur = begin_;
        while (cur_cp->next) {
            cur_cp = cur_cp->next;
            InsertAfter(cur, new Node(cur_cp->val));
            cur = cur->next;
        }
    }

    List(List&& list)
        : begin_(std::move(list.begin_)), end_(std::move(list.end_)), size_(list.size_) {
        list.size_ = 0;
        list.begin_ = nullptr;
        list.end_ = nullptr;
    }

    ~List() {
        while (!IsEmpty()) {
            --size_;
            Erase(end_);
        }
    }

    List& operator=(const List& list) {
        if (this == &list) {
            return *this;
        }
        this->~List();
        new (this) List(list);
        return *this;
    }

    List& operator=(List&& list) {
        if (this == &list) {
            return *this;
        }
        this->~List();
        new (this) List(std::move(list));
        return *this;
    }

    bool IsEmpty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    void PushBack(const T& elem) {
        InsertAfter(end_, new Node(elem));
        ++size_;
    }

    void PushBack(T&& elem) {
        InsertAfter(end_, new Node(std::move(elem)));
        ++size_;
    }

    void PushFront(const T& elem) {
        InsertBefore(begin_, new Node(elem));
        ++size_;
    }

    void PushFront(T&& elem) {
        InsertBefore(begin_, new Node(std::move(elem)));
        ++size_;
    }

    T& Front() {
        return begin_->val;
    }

    const T& Front() const {
        return begin_->val;
    }

    T& Back() {
        return end_->val;
    }

    const T& Back() const {
        return end_->val;
    }

    void PopBack() {
        --size_;
        Erase(end_);
    }

    void PopFront() {
        --size_;
        Erase(begin_);
    }

    Iterator Begin() {
        return Iterator(begin_, end_);
    }

    Iterator End() {
        return Iterator(nullptr, end_);
    }

private:
    Node* begin_;
    Node* end_;
    size_t size_;

    void InsertBefore(Node* existing_node, Node* new_node) {
        if (begin_ == nullptr) {
            begin_ = end_ = new_node;
        } else if (existing_node == begin_) {
            new_node->next = existing_node;
            existing_node->prev = new_node;
            begin_ = new_node;
        } else {
            Node* ex_previous_node = existing_node->prev;
            existing_node->prev = new_node;
            new_node->prev = ex_previous_node;
            ex_previous_node->next = new_node;
            new_node->next = existing_node;
        }
    }

    void InsertAfter(Node* existing_node, Node* new_node) {
        if (IsEmpty()) {
            begin_ = end_ = new_node;
        } else if (existing_node == end_) {
            existing_node->next = new_node;
            new_node->prev = existing_node;
            end_ = new_node;
        } else {
            Node* ex_next = existing_node->next;
            existing_node->next = new_node;
            new_node->prev = existing_node;
            ex_next->prev = new_node;
            new_node->next = ex_next;
        }
    }

    void Erase(Node* node) {
        if (node->next == nullptr && node->prev == nullptr) {
            delete node;
            begin_ = end_ = nullptr;
        } else if (node->next == nullptr) {
            Node* previous = node->prev;
            delete node;
            end_ = previous;
            end_->next = nullptr;
        } else if (node->prev == nullptr) {
            Node* next = node->next;
            delete node;
            begin_ = next;
            begin_->prev = nullptr;
        } else {
            Node* previous = node->prev;
            Node* next = node->next;
            previous->next = next;
            next->prev = previous;
            delete node;
        }
    }
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {
    return list.End();
}

