#pragma once

template <class T>
class Ptr {
  public:
  Ptr() = default;

  Ptr(void* data) {
    assert(Data == nullptr);
    Data = static_cast<T*>(data);
  }

  Ptr(const Ptr&) = delete;
  Ptr&
  operator=(const Ptr&) = delete;
  Ptr(Ptr&&) = delete;
  Ptr&
  operator=(Ptr&&) = delete;

  template <class... Args>
  void
  New(Args&&... _args) {
    assert(Data == nullptr);
    Data = new T(std::forward<Args>(_args)...);
  }

  T*
  operator->() {
    assert(Data != nullptr);
    return Data;
  }

  T&
  operator*() {
    assert(Data != nullptr);
    return *Data;
  }

  ~Ptr() {
    assert(Data != nullptr);
    delete Data;
  }

  private:
  T* Data = nullptr;
};
