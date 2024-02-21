#pragma once

#include <array>

template <typename _Type, uint32_t _size>
struct ArithmeticAverager {
  // use smallest type to track index
  using _Index = std::conditional_t<_size <=  0x100, uint8_t, std::conditional_t<_size <= 0x10000, uint16_t, uint32_t > >;
  typedef _Index Index;
  typedef _Type Type;
  static const uint32_t size = _size;
  
  _Index at;
  _Type sum;
  std::array<_Type, _size> samples;

  ArithmeticAverager(const _Type &value=_Type(0)) : at(0), sum(_size*value) { samples.fill(value); }

  _Type average() const { return sum/_size; }
  void sample(const _Type &value) {
    sum += value - samples[at];
    samples[at]=value;
    at = (at + 1) % _size;
  }

  void reset(const _Type &value) { at=0; sum=_size*value; samples.fill(value); }
};

template <typename _Type, uint32_t _size>
struct GeometricAverager {
  typedef _Type Type;
  static const uint32_t size = _size;
  _Type sum;

  GeometricAverager(const _Type &value=_Type(0)) : sum((_size*_size)*value) {}

  _Type average() const { return sum/(_size*_size); }
  void sample(const _Type &value) {
    sum += _size*value - sum/_size;
  }

  void operator=(const _Type &value) { sum=(_size*_size)*value; }
};
