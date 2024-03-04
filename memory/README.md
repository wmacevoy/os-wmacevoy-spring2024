# Randomize

If you want a block of cryptographically strong random bits,
```C++
   uint8_t buffer[128];
   randomize(buffer,sizeof(buffer));

   std::array < uint32_t , 20 > buf32;
   randomize(buf32);

   uint64_t seed = randomize<uint64_t>();
```
# Canary
On process creation, this creates a 64 bit random reference value.  `Canary here` adds a local canary value that can be `.check()` or checked automatically on destruction.

An optimizing compiler may move the location of the canary, so you should use it as "bookmarks" in SecureData and SecureArray.

# SecureData
```C++
SecureData < Type , Reset = [](Type &data) { data = Type(); } > secData;
```
Creates a Type data that is reset on construction and destruction, has
pages locked while in scope, and is surrounded by two canaries (they may be not quite adjacent because of word alignment).

# SecureArray+
```C++
SecureArray < Type , Size , Reset = [](std::array<Type,Size> &data) { data.fill(Type()); }> secArray;
```
SecureData specialized to an array.


