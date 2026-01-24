#pragma once

template <typename T>
class IotaIterator {
  public:
  IotaIterator(T v);

  T
  operator*();

  void
  operator++();

  bool
  operator!=(const IotaIterator<T> o);

  private:
  T v;
};

template <typename T>
class Iota {
  public:
  Iota(T end);

  IotaIterator<T>
  begin();

  IotaIterator<T>
  end();

  private:
  T v;
};

template <typename T>
IotaIterator<T>::IotaIterator(T v) : v(v) {
}

template <typename T>
T
IotaIterator<T>::operator*() {
  return v;
}

template <typename T>
void
IotaIterator<T>::operator++() {
  v++;
}

template <typename T>
bool
IotaIterator<T>::operator!=(const IotaIterator<T> o) {
  return v != o.v;
}

template <typename T>
Iota<T>::Iota(T end) : v(end) {
}

template <typename T>
IotaIterator<T>
Iota<T>::begin() {
  return 0;
}

template <typename T>
IotaIterator<T>
Iota<T>::end() {
  return v;
}
