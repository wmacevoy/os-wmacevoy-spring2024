#include <iostream>
#include <math.h>
#include <assert.h>
#include "averager.h"

void testArithmeticAverager() {
  const double samplesPerSecond = 1'000.0;
  const double frequency = 4.0;
  const double omega = 2*M_PI*frequency;
  const int samples = 1'000;

  ArithmeticAverager<double,32> averager;

  for (int it=0; it<samples; ++it) {
    double t = double(it)/samplesPerSecond;
    double value = sin(omega*t);
    averager.sample(value);
  }

  // explicit average to compare
  double sum = 0;
  for (int it=samples-averager.size; it < samples; ++it) {
    double t = double(it)/samplesPerSecond;
    double value = sin(omega*t);
    sum += value;
  }
  double average = sum/averager.size;

  assert(fabs(average-averager.average()) < 1e-9);

  std::cout << "arithmetic average is " << averager.average() << std::endl;
}

int main() {
  testArithmeticAverager();
}
