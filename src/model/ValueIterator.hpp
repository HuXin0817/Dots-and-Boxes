#pragma once

namespace dab::model {

template <typename Model, typename SizeType>
class ValueIterator {
  struct iterator {
    iterator(SizeType v) : v(v) {
    }
    Model
    operator*() {
      return v;
    }
    void
    operator++() {
      v++;
    }
    bool
    operator!=(iterator o) {
      return v != o.v;
    }

    SizeType v;
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
