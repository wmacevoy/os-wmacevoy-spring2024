#pragma once

#include <concepts>
#include <array>

template<typename T>
concept MathematicalField = requires(const T a, const T b) {
    { T(0) } -> std::same_as<T>;
    { T(1) } -> std::same_as<T>;
    { a + b } -> std::same_as<T>;
    { a - b } -> std::same_as<T>;
    { a * b } -> std::same_as<T>;    
    { a / b } -> std::same_as<T>;
};

template <MathematicalField _Type, uint32_t _size>
struct ArithmeticAverager {
  using Type = _Type;
  static constexpr uint32_t size = _size;
  // use smallest type to track index
  using Index = std::conditional_t<_size <=  0x100, uint8_t, std::conditional_t<_size <= 0x10000, uint16_t, uint32_t > >;
  
  Index at;
  Type sum;
  std::array<Type, _size> samples;

  ArithmeticAverager(const _Type &value=_Type(0)) : at(0), sum(_Type(_size)*value) { samples.fill(value); }

  _Type average() const { return sum/_size; }
  void sample(const _Type &value) {
    sum = sum + (value - samples[at]);
    samples[at]=value;
    at = (at + 1) % _size;
  }

  void reset(const _Type &value) { at=0; sum=_size*value; samples.fill(value); }
};

template <MathematicalField _Type, uint32_t _size>
struct GeometricAverager {
  using Type = _Type;
  static constexpr _Type size = _Type(_size);
  static constexpr _Type size2 = _Type(_size)*_Type(_size);
  _Type sum;

  GeometricAverager(const _Type &value=_Type(0)) : sum(size2*value) {}

  _Type average() const { return sum/size2; }
  void sample(const _Type &value) {
    sum = sum + (size*value - sum/size);
  }

  void operator=(const _Type &value) { sum=size2*value; }
};
