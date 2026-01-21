#pragma once

namespace dab::model {

template <typename Model>
class ValueIterator {
  struct iterator {
    iterator(int v) : v(v) {
    }
    Model
    operator*() {
      return v;
    }
    iterator&
    operator++() {
      v++;
      return *this;
    }
    bool
    operator!=(iterator o) {
      return v != o.v;
    }

    int v;
  };

  public:
  ValueIterator() = default;

  iterator
  begin() {
    return 0;
  }

  iterator
  end() {
    return Model::Max;
  }

  iterator
  begin() const {
    return 0;
  }

  iterator
  end() const {
    return Model::Max;
  }
};

}  // namespace dab::model
