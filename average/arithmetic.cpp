// needed for template
#include <stdint.h>
#include <array>

// needed for example
#include <iostream>
#include <math.h>
#include <assert.h>

template <typename _Type, uint32_t _size>
struct ArithmeticAverager {
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
    sum = sum + value - samples[at];
    samples[at]=value;
    at = (at + 1) % _size;
  }

  void operator=(const _Type &value) { sum=_size*value; samples.fill(value); }
};

void testArithmeticAverager() {
  typedef ArithmeticAverager<double,32> Averager;
  Averager averager;

  assert(typeid(Averager::Type)==typeid(double));
  assert(Averager::size == 32);
  assert(typeid(Averager::Index)==typeid(uint8_t));
  assert(sizeof(Averager::samples) == Averager::size*sizeof(Averager::Type));

  const double samplesPerSecond = 1'000.0;
  const double frequency = 440.0;
  const double omega = 2*M_PI*frequency;
  const int samples = 1'000;

  for (int it=0; it<samples; ++it) {
    double t = double(it)/samplesPerSecond;
    double sample = sin(t*2*M_PI/frequency);
    double value = sin(omega*t);
    averager.sample(value);
  }

  // explicit average to compare
  double sum = 0;
  for (int it=samples-averager.size; it < samples; ++it) {
    double t = double(it)/samplesPerSecond;
    double sample = sin(t*2*M_PI/frequency);
    double value = sin(omega*t);
    sum += value;
  }
  double average = sum/averager.size;

  assert(fabs(average-averager.average()) < 1e-9);
}

int main() {
  testArithmeticAverager();
}
