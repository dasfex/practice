#include <type_traits>
#include <vector>

template<class T, class Func>
class UpdQueue{
 public:
  template<class U, class = std::enable_if<std::is_convertible_v<std::decay_t<U>,
                                                                 std::decay_t<Func>>>>
  explicit UpdQueue(U&& func) : func(func) {}

  template<class U, class = std::enable_if<std::is_convertible_v<std::decay_t<U>, std::decay_t<T>>>>
  void push(U&& val) {
    if (give.empty()) {
      give.emplace_back(val, val);
    } else {
      give.emplace_back(val, func(val, give.back().second));
    }
  }

  T get_max() {
    if (give.empty()) {
      return get.back().second;
    }
    if (get.empty()) {
      pass();
      return get.back().second;
    }
    return func(get.back().second, give.back().second);
  }

  void pop() {
    pass();
    get.pop_back();
  }

 private:
  Func func;
  std::vector<std::pair<T, T>> get;
  std::vector<std::pair<T, T>> give;

  void pass() {
    if (!get.empty()) { return; }
    while (!give.empty()) {
      T val_in = give.back().first;
      if (get.empty()) {
        get.emplace_back(val_in, val_in);
      } else {
        get.emplace_back(val_in, func(val_in, get.back().second));
      }
      give.pop_back();
    }
  }
};
