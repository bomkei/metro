#pragma once

template <class T>
struct ListWrap {
  std::list<T> list;

  void push(T const& item) {
    list.push_front(item);
  }

  void pop() {
    list.pop_front();
  }

  T& get() {
    return *list.begin();
  }

  void clear() {
    list.clear();
  }
};
