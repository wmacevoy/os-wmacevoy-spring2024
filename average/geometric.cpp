// needed for template
#include <stdint.h>

// needed for example
#include <iostream>
#include <math.h>
#include <assert.h>

template <typename _Type, uint32_t _size>
struct GeometricAverager {
  typedef _Type Type;
  static const uint32_t size = _size;
  _Type sum;

  GeometricAverager(const _Type &value=_Type(0)) : sum(_size*_size*value) {}

  _Type average() const { return sum/(_size*_size); }
  void sample(const _Type &value) {
    sum += _size*value - sum/_size;
  }

  void operator=(const _Type &value) { sum=(_size*_size)*value; }
};

void testGeometricAverager() {
  typedef GeometricAverager<double,32> Averager;
  Averager averager;

  assert(typeid(Averager::Type)==typeid(double));
  assert(Averager::size == 32);

  const double samplesPerSecond = 1'000.0;
  const double frequency = 440.0;
  const double omega = 2*M_PI*frequency;
  const int samples = 1'000;

  for (int it=0; it<samples; ++it) {
    double t = double(it)/samplesPerSecond;
    double value = sin(omega*t);
    averager.sample(value);
  }

  // explicit geometric sum to compare
  double sum = 0;
  double r = 1.0-1.0/Averager::size;
  double A = 0;
  
  for (int it=0; it < samples; ++it) {
    double t = double(it)/samplesPerSecond; 
    double value = sin(omega*t);
    
    int age = samples-1 - it;
    double w = pow(r,age);
    sum = sum + w*value;
    A = A + w;
  }
  double average = sum/A;

  assert(fabs(average-averager.average()) < 1e-9);
}

int main() {
  testGeometricAverager();
}
