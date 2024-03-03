#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib") // Link against the bcrypt library
template <typename Primitive>
void randomize(Primitive &primitive) {
  if (BCryptGenRandom(NULL, reinterpret_cast<PUCHAR>(&primitive), sizeof(Primitive), BCRYPT_USE_SYSTEM_PREFERRED_RNG) != ERROR_SUCCESS) {
    std::abort();
  }
}
#else
#include <fstream>
template <typename Primitive>
void randomize(Primitive &primitive) {
    static std::ifstream random("/dev/random", std::ios::in | std::ios::binary);
    if (!random.read(reinterpret_cast<char*>(&primitive), sizeof(Primitive))) {
      std::abort();
    }
}
#endif

template <typename Primitive> Primitive randomize() {
    Primitive primitive;
    randomize(primitive);
    return primitive;
}
