#pragma once
#include <stddef.h>

// Fill data with size bytes of cryptographically strong randomness
//
// NOTE:
//
//   Your data type better not have pointers or virtual functions/methods,
//   for they will surely be corrupted
//
// Ex:
//   uint8_t noise[4096];
//   randomize(noise,sizeof(noise));
//
void randomize(void *data, size_t size);

// Template version so you don't have to pass in size
// Ex:
//   std::array<uint8_t, 4096> buffer;
//   randomize(noise);
//
template <typename Simple>
void randomize(Simple &simple) {
  randomize(&simple, sizeof(Simple));
}


// Template version so you can initialize a value
// Ex:
//   uint64_t seed = randomize<uint64_t>();
//
template <typename Primitive> Primitive randomize() {
    Primitive primitive;
    randomize(primitive);
    return primitive;
}
