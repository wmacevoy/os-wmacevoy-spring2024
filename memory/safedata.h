#include "canary.h"

void SecureDataLock(void *base, size_t len);
void SecureDataUnlock(void *base, size_t len);   


template<typename _Type, Function<void (_Type &data)> Reset = [] { data=_Type(); } >
class SecureData {
 public: using Type = _Type;
 public: Canary before;
 public: _Type data;
 public: Canary after;

 public: void check() {
    before.check(); 
    after.check(); 
  }
  
 public: void reset() { 
    Reset(data);
    check();
  }

  
 public: SecureData() {
   SecureDataLock(&data,sizeof(Data));
   Reset(data);
 }
  
 public: ~SecureData() { 
    Reset(data);
    SecureDataUnlock(&data,sizeof(Data));
  }
};
