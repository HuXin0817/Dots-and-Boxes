#pragma once

template <class T>
class Span {
  public:
  Span() = default;

  Span(const T* begin, const T* end) : Begin(begin), End(end) {
  }

  const T*
  begin() const {
    return Begin;
  }

  const T*
  end() const {
    return End;
  }

  int
  Size() const {
    return End - Begin;
  }

  const T&
  At(int i) const {
    assert(i >= 0 && i < Size());
    return Begin[i];
  }

  bool
  Empty() const {
    return End == Begin;
  }

  private:
  const T* Begin = nullptr;
  const T* End = nullptr;
};
