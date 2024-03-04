#include <stdint.h>
#include <string.h>
#include <array>
#include <iostream>

#include "securedata.h"

struct Key {
  static constexpr size_t size = 8;
  int mode;
  std::array<uint8_t,size> sec;
  std::array<uint8_t,size> pub;
  static void reset(Key &key) {
    key.mode = 0;
    key.sec.fill(0);
    key.pub.fill(0);
  }
};

void checked(bool ok) {
  SecureData<Key,Key::reset> secureKey;
  char *sec = (char*) &secureKey.data.sec[0];
  char *pub = (char*) &secureKey.data.pub[0];
  
  if (ok) {
    strncpy(sec,"clipped secret key",Key::size);
    strncpy(pub,"clipped public key",Key::size);
  } else {
    strcpy(sec,"overflow secret key");
    strcpy(pub,"overflow public key");
  }
}

int main(int argc, const char *argv[]) {
  bool ok = (argc > 1 && std::strcmp(argv[1],"--ok=true") == 0);
  std::cout << argv[0] << " " << "--ok=" << (ok ? "true" : "false") << std::endl;
  checked(ok);
  return 0;
}
