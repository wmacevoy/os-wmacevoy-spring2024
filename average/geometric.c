#include <stdio.h>
#include <math.h>

#define SIZE 32
double sum = 0.0;

void sample(double value) {
  sum += SIZE*value - sum/SIZE;
}

double average() {
  return sum/(SIZE*SIZE);
}

void reset(double value) {
  sum = (SIZE*SIZE)*value;
}

int main() {
  double samplesPerSecond = 1000.0;
  double frequency = 4.0;
  double omega = 2*M_PI*frequency;
  int samples = 1000;

  for (int it=0; it<samples; ++it) {
    double t = ((double) it)/samplesPerSecond;
    double value = sin(omega*t);
    sample(value);
  }

  printf("geometric average (size %d) is %g\n",SIZE,average());

  return 0;
}
