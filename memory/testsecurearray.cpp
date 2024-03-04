#include <iostream>
#include <stdint.h>
#include <string.h>

#include "securearray.h"

void checked(bool ok) {
  constexpr size_t size = 32;
  SecureArray<uint32_t,size> secvec;
  if (ok) {
    uint32_t *a=(uint32_t*)&secvec.data[0];
    for (int i=0; i<size; ++i) {
      a[i]=i;
    }
  } else {
    uint64_t *a=(uint64_t*)&secvec.data[0];
    for (int i=0; i<size; ++i) {
      a[i]=i;
    }
  }
}

int main(int argc, const char *argv[]) {
  bool ok = (argc > 1 && strcmp(argv[1],"--ok=true") == 0);
  std::cout << argv[0] << " " << "--ok=" << (ok ? "true" : "false") << std::endl;
  checked(ok);
  return 0;
}
