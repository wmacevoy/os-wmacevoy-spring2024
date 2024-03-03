#pragma once

#include "securedata.h"
#include <array>

template <typename T,size_t Size>
  struct SecureArrayReset {
    void operator()(std::array < T , Size > &data) { data.fill(T()); }
  };

// SecureArray is SecureData with a reset that fills with default T values
template <typename T,size_t Size>
  using SecureArray = SecureData < std::array < T , Size >, SecureArrayReset<T,Size> >;
