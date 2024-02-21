#include <iostream>
#include <math.h>
#include <assert.h>
#include "averager.h"

void testGeometricAverager() {
  const double samplesPerSecond = 1'000.0;
  const double frequency = 4.0;
  const double omega = 2*M_PI*frequency;
  const int samples = 1'000;

  GeometricAverager<double,32> averager;

  for (int it=0; it<samples; ++it) {
    double t = double(it)/samplesPerSecond;
    double value = sin(omega*t);
    averager.sample(value);
  }

  // explicit geometric sum to compare
  double sum = 0;
  double r = 1.0-1.0/averager.size;
  double A = 0;
  
  for (int it=0; it < samples; ++it) {
    double t = double(it)/samplesPerSecond; 
    double value = sin(omega*t);
    
    int age = samples-1 - it;
    double w = pow(r,age);
    sum = sum + w*value;
    A = A + w;
  }

  A = 1/A;
  double average = A*sum;

  assert(fabs(average-averager.average()) < 1e-9);

  std::cout << "geometric average is " << averager.average() << std::endl;  
}

int main() {
  testGeometricAverager();
}
