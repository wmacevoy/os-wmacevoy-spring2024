#include <functional>

#include "canary.h"

void SecureDataLock(void *base, size_t len);
void SecureDataUnlock(void *base, size_t len);   

// default template reset for data type (reset to default)
template <typename Type>
void SecureDataReset(Type &data) {
  data = Type();
}


//
// Wrap a Type between Canary values and that is locked out of paging.
// They can be checked at any time, but are automatically reset & checked on destruction.
//
template<typename _Type, auto _Reset = &SecureDataReset<_Type> >
class SecureData {
 public: using Type = _Type;
 public: const Canary before;
 public: _Type data;
 public: const Canary after;

 public: void check() const {
    before.check(); 
    after.check(); 
  }

 public: void reset() {
    //    _Reset reset;
    _Reset(data);
    before.check();
    after.check();
  }
  
 public: SecureData() {
    SecureDataLock(&data,sizeof(_Type));
    reset();
  }
  
 public: ~SecureData() { 
    reset();
    SecureDataUnlock(&data,sizeof(_Type));
  }
};

