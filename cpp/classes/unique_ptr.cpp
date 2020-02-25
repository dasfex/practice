#include <iostream>
#include <cassert>
#include <string>
#include <typeinfo>
#include <vector>
#include <utility>

using namespace std;

template<typename T>
class UniquePtr {
 public:
  UniquePtr() : ptr_(nullptr) {}

  explicit UniquePtr(T* ptr) : ptr_(ptr) {}

  UniquePtr(const UniquePtr&) = delete;
  UniquePtr& operator=(const UniquePtr&) = delete;

  UniquePtr(UniquePtr&& other) : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }

  UniquePtr& operator=(UniquePtr&& other) {
    delete ptr_;
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    return *this;
  }

  ~UniquePtr() {
    delete ptr_;
  }

  T* Get() {
    return ptr_;
  }

  const T* Get() const {
    return ptr_;
  }

  void Release() {
    ptr_ = nullptr;
  }

  T& operator*() {
    return *ptr_;
  }

  T* operator->() {
    return ptr_;
  }

  const T& operator*() const {
    return *ptr_;
  }

  const T* operator->() const {
    return ptr_;
  }

 private:
  T* ptr_;
};

template<typename T>
class UniquePtr<T[]> {
 public:
  UniquePtr() : ptr_(nullptr) {}

  explicit UniquePtr(T* ptr) : ptr_(ptr) {}

  UniquePtr(const UniquePtr&) = delete;
  UniquePtr& operator=(const UniquePtr&) = delete;

  UniquePtr(UniquePtr&& other) : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }

  UniquePtr& operator=(UniquePtr&& other) {
    delete[] ptr_;
    ptr_ = other.ptr_;
    other.ptr_ = nullptr;
    return *this;
  }

  ~UniquePtr() {
    delete[] ptr_;
  }

  T* Get() {
    return ptr_;
  }

  const T* Get() const {
    return ptr_;
  }

  void Release() {
    ptr_ = nullptr;
  }

  T& operator*() {
    return *ptr_;
  }

  T* operator->() {
    return ptr_;
  }

  const T& operator*() const {
    return *ptr_;
  }

  const T* operator->() const {
    return ptr_;
  }

  T& operator[](int ind) {
    return ptr_[ind];
  }

  const T& operator[](int ind) const {
    return ptr_[ind];
  }

 private:
  T* ptr_;
};

#ifndef IGNORE_MAIN

template<typename T>
std::string TypeName() {
  using T_no_ref = typename std::remove_reference<T>::type;
  std::string r;
  if (std::is_const<T_no_ref>::value) {
    r += "const ";
  }
  if (std::is_volatile<T_no_ref>::value) {
    r += "volatile ";
  }
  r += typeid(T_no_ref).name();
  if (std::is_lvalue_reference<T>::value) {
    r += "&";
  }
  if (std::is_rvalue_reference<T>::value) {
    r += "&&";
  }
  return r;
}

int main() {
  UniquePtr<int> ptr;
#ifndef SKIP_SINGLE
  {
    UniquePtr<int> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(TypeName<decltype(i_ptr.Get())>() == "Pi");
  }
  {
    const UniquePtr<int> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(TypeName<decltype(i_ptr.Get())>() == "PKi");
  }
  {
    auto* i = new int(42);
    UniquePtr<int> i_ptr(i);
    assert(i_ptr.Get() == i);
    i_ptr.Release();
    assert(i_ptr.Get() == nullptr);
    delete i;
  }
  {
    auto* i = new int(42);
    UniquePtr<int> i_ptr(i);

    assert(*i_ptr == 42);

    *i = 123;
    assert(*i_ptr == 123);

    assert(TypeName<decltype(*i_ptr)>() == "i&");
    assert(TypeName<decltype(i_ptr.operator->())>() == "Pi");

    const auto& const_i_ptr = i_ptr;
    assert(TypeName<decltype(*const_i_ptr)>() == "const i&");
    assert(TypeName<decltype(const_i_ptr.operator->())>() == "PKi");
  }
  {
    auto* s = new string("Hello, ");
    UniquePtr<string> s_ptr(s);

    assert(s_ptr->length() == 7);
    assert(*s_ptr == "Hello, ");

    *s_ptr += "world";
    assert(s_ptr->length() == 12);
    assert(*s_ptr == "Hello, world");

    *s += "!";
    assert(s_ptr->length() == 13);
    assert(*s_ptr == "Hello, world!");

    assert(TypeName<decltype(*s_ptr)>() == "Ss&");
    assert(TypeName<decltype(s_ptr.operator->())>() == "PSs");
  }
  {
    auto* s = new string("Hello");
    const UniquePtr<string> s_ptr(s);

    assert(s_ptr->length() == 5);
    assert(*s_ptr == "Hello");

    *s += "!";
    assert(s_ptr->length() == 6);
    assert(*s_ptr == "Hello!");

    assert(TypeName<decltype(*s_ptr)>() == "const Ss&");
    assert(TypeName<decltype(s_ptr.operator->())>() == "PKSs");
  }
  {
    auto* s = new vector<string>({"Hello", "World"});
    const UniquePtr<vector<string>> s_ptr(s);

    assert(s_ptr->size() == 2);
    assert((*s_ptr)[0] == "Hello");
    assert((*s_ptr)[1] == "World");

    assert(TypeName<decltype((*s_ptr)[0])>() == "const Ss&");
  }
  {
    UniquePtr<string> ptr1(new string("42"));
    assert(*ptr1 == "42");

    UniquePtr<string> ptr2(move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(*ptr2 == "42");
  }
  {
    UniquePtr<string> ptr1(new string("42"));
    assert(*ptr1 == "42");

    UniquePtr<string> ptr2(new string("11111"));
    assert(*ptr1 == "42");
    assert(*ptr2 == "11111");

    ptr2 = std::move(ptr1);
    assert(ptr1.Get() == nullptr);
    assert(*ptr2 == "42");

    auto ret_type = TypeName<decltype(ptr2 = std::move(ptr1))>();
    assert(ret_type == string(typeid(UniquePtr<string>).name()) + "&");
  }
  {
    UniquePtr<string> ptr1(new string("42"));
    assert(*ptr1 == "42");

    UniquePtr<string> ptr2(new string("11111"));
    assert(*ptr1 == "42");
    assert(*ptr2 == "11111");

    UniquePtr<string> ptr3 = move(ptr2 = std::move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() == nullptr);
    assert(*ptr3 == "42");
  }
  cout << "OK: single object" << endl;
#else
  cout << "Skipped: single object" << endl;
#endif

#ifndef SKIP_ARRAY
  {
    UniquePtr<int[]> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(TypeName<decltype(i_ptr.Get())>() == "Pi");
  }
  {
    const UniquePtr<int[]> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(TypeName<decltype(i_ptr.Get())>() == "PKi");
  }
  {
    auto* i = new int[42];
    UniquePtr<int> i_ptr(i);
    assert(i_ptr.Get() == i);
    i_ptr.
        Release();
    assert(i_ptr.Get() == nullptr);
    delete[]
        i;
  }
  {
    auto* i = new int[4];
    i[0] = 1;
    i[2] = 654321;

    UniquePtr<int[]> i_ptr(i);
    assert(*i_ptr == 1);
    assert(i_ptr[2] == 654321);

    i[2] = 11111;
    assert(i_ptr[2] == 11111);

    i_ptr[2] /= 100;
    assert(i_ptr[2] == 111);

    assert(TypeName<decltype(*i_ptr)>() == "i&");
    assert(TypeName<decltype(i_ptr.operator->())>() == "Pi");
    assert(TypeName<decltype(i_ptr.operator[](2))>() == "i&");
  }
  {
    auto* s = new string[4];
    s[0] = "First";
    s[2] = "Hi ";

    UniquePtr<string[]> s_ptr(s);
    assert(*s_ptr == "First");
    assert(s_ptr[2] == "Hi ");

    s[2] += "World";
    assert(s_ptr[2] == "Hi World");

    s_ptr[2] += "!!!";
    assert(s_ptr[2] == "Hi World!!!");

    assert(TypeName<decltype(*s_ptr)>() == "Ss&");
    assert(TypeName<decltype(s_ptr.operator->())>() == "PSs");
    assert(TypeName<decltype(s_ptr.operator[](2))>() == "Ss&");
  }
  {
    auto* i = new int[4];
    i[0] = 1;
    i[2] = 654321;

    const UniquePtr<int[]> i_ptr(i);
    assert(*i_ptr == 1);
    assert(i_ptr[2] == 654321);

    i[2] = 11111;
    assert(i_ptr[2] == 11111);

    assert(TypeName<decltype(*i_ptr)>() == "const i&");
    assert(TypeName<decltype(i_ptr.operator->())>() == "PKi");
    assert(TypeName<decltype(i_ptr.operator[](2))>() == "const i&");
  }
  {
    auto* s = new string[4];
    s[0] = "First";
    s[2] = "Hi ";

    const UniquePtr<string[]> s_ptr(s);
    assert(*s_ptr == "First");
    assert(s_ptr[2] == "Hi ");

    s[2] += "World";
    assert(s_ptr[2] == "Hi World");

    assert(TypeName<decltype(*s_ptr)>() == "const Ss&");
    assert(TypeName<decltype(s_ptr.operator->())>() == "PKSs");
    assert(TypeName<decltype(s_ptr.operator[](2))>() == "const Ss&");
  }
  {
    UniquePtr<string[]> ptr1(new string[2]);
    assert(ptr1.Get() != nullptr);

    UniquePtr<string[]> ptr2(move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() != nullptr);
  }
  {
    UniquePtr<string[]> ptr1(new string[2]);
    assert(ptr1.Get() != nullptr);

    UniquePtr<string[]> ptr2(new string[2]);
    assert(ptr1.Get() != nullptr);
    assert(ptr2.Get() != nullptr);

    ptr2 = std::move(ptr1);
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() != nullptr);

    auto ret_type = TypeName<decltype(ptr2 = std::move(ptr1))>();
    assert(ret_type == string(typeid(UniquePtr<string[]>).name()) + "&");
  }
  {
    UniquePtr<string[]> ptr1(new string[2]);
    assert(ptr1.Get() != nullptr);

    UniquePtr<string[]> ptr2(new string[2]);
    assert(ptr1.Get() != nullptr);
    assert(ptr2.Get() != nullptr);

    UniquePtr<string[]> ptr3 = move(ptr2 = std::move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() == nullptr);
    assert(ptr3.Get() != nullptr);
  }
  cout << "OK: array" << endl;
#else
  cout << "Skipped: array" << endl;
#endif

  return 0;
}
#endif
