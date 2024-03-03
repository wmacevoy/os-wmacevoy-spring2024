#include <cstdlib>
#include <cstdint>
#include <exception>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/mman.h>
#endif

#include "securedata.h"

void SecureDataLock(void *data, size_t size) {
#ifdef _WIN32
  if (!VirtualLock(data,size)) {
    throw std::bad_alloc();
  }
#else
  if (mlock(data,size) != 0) {
    throw std::bad_alloc();
  }
#endif
}

void SecureDataUnlock(void *data, size_t size) {
#ifdef _WIN32
  if (!VirtualUnlock(data,size)) {
    std::abort();
  }
#else
  if (munlock(data,size) != 0) {
    std::abort();
  }
#endif
}
