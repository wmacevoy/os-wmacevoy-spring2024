#pragma once
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <array>
#include <sys/mman.h>
#include <exception>
#include <iostream>

// destructor automatically aborts if canary value is
// tampered with.
class Canary {
    public: using Type = uint64_t;

    // initialized to random value once
    private: static const Type reference;

    // set to reference value by constructor,
    // check by destructor
    private: const Type instance;

    // initialize instance
    public: inline Canary() : instance(reference) {}

    // test if uncorrupted
    public: inline bool alive() const {
        return instance == reference;
    }

    // abort if corrupted
    public: inline void check() {
         if (!alive()) {
            std::abort();
         }
    }

    // automatically check on destruction
    public: inline ~Canary() { check(); }
};

template<typename T,size_t Size>
class SecureArray {
    public: Canary before;
    public: std::array<T,Size> data;
    public: Canary after;

    public: void check() {
      before.check(); 
      after.check(); 
    }
  
    public: void reset() { 
      data.fill(T());
      check();
    }

    private: void lock() {
#ifdef _WIN32
      if (!VirtualLock(&data,sizeof(T)*Size)) {
        throw std::bad_alloc();
      }
#else
      if (mlock(&data,sizeof(T)*Size)!=0) {
        throw std::bad_alloc();
      }
#endif
    }

    private: void unlock() {
#ifdef _WIN32
      if (!VirtualUnlock(&data,sizeof(T)*Size)) {
        std::abort();
      }
#else
      if (munlock(&data,sizeof(T)*Size)!=0) {
        std::abort();
      }
#endif
    }

    public: SecureArray() {
      lock();
      data.fill(T()); 
    }

    public: ~SecureArray() { 
        data.fill(T());
        unlock();
    }
};
