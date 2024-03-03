#pragma once
#include <cstdint>
#include <cstdlib>

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
    public: inline void check() const {
         if (!alive()) {
            std::abort();
         }
    }

    // automatically check on destruction
    public: inline ~Canary() { check(); }
};
