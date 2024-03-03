#include <iostream>
#include <cmath>
#include <cassert>

#include "randomize.h"

void testRandomize() {
  int m = 256;
  uint32_t bins[256];
  for (int i=0; i<m; ++i) bins[i]=0;
  
  int n=1000*m;
  for (int i=0; i<n; ++i) {
    uint8_t r=randomize<uint8_t>();
    ++bins[r];
  }

  double p = 1.0/m;
  double q = 1-p;
  double expect = p*n;
  double sum=0,sum2=0;

  for (int i=0; i<m; ++i) {
    double x=bins[i]-expect;
    sum += x;
    sum2 += std::pow(x,2);
  }

  double smean = sum / m;
  double sstddev = std::sqrt(sum2/m);
  double emean = 0;
  double estddev = std::sqrt(n*p*q);
  double z = (sstddev-estddev)/(estddev/sqrt(m));
  
  std::cout << "smean=" << smean << " sstddev=" << sstddev << std::endl;
  std::cout << "emean=" << emean << " estddev=" << estddev << std::endl;
  std::cout << "z=" << z << std::endl;  
  assert(fabs(z) < 6);
}

int main() {
  testRandomize();
  return 0;
}
