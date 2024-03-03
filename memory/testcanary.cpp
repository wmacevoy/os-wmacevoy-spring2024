#include <iostream>

#include "canary.h"

void testCanary(int b, int e) {
    SecureArray<int,10> buffer;
    for (int i=b; i<e; ++i) {
      buffer.data[i]=i;
    }
}

int main(int argc, const char *argv[]) {
    int b = argc > 2 ? atoi(argv[1]) : 0;
    int e  = argc > 1 ? atoi(argv[argc-1]) : 10;
    std::cout << "b=" << b << " e=" << e << std::endl;
    testCanary(b,e);
    return 0;
}
