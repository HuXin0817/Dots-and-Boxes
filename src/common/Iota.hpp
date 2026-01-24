#pragma once

template <typename T>
class IotaIterator {
  public:
  IotaIterator(T v) : v(v) {
  }

  T
  operator*() {
    return v;
  }

  void
  operator++() {
    v++;
  }

  bool
  operator!=(const IotaIterator<T> o) {
    return v != o.v;
  }

  private:
  T v;
};

template <typename T>
class Iota {
  public:
  Iota(T end) : v(end) {
  }

  IotaIterator<T>
  begin() {
    return 0;
  }

  IotaIterator<T>
  end() {
    return v;
  }

  private:
  T v;
};
