#include <stdio.h>
#include <math.h>

#define SIZE 32
double samples[SIZE] = {0.0};
int at = 0;
double sum = 0.0;

void sample(double value) {
  sum += value - samples[at];
  samples[at]=value;
  at = (at + 1) % SIZE;
}

double average() {
  return sum/SIZE;
}

void reset(double value) {
  at=0;
  sum = SIZE*value;
  for (int i=0; i<SIZE; ++i) {
    samples[i]=value;
  }
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

  printf("arithmetic average (size %d) is %g\n",SIZE,average());

  return 0;
}
