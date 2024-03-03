#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <cassert>

#include "securearray.h"

void checked(bool ok) {
  constexpr size_t size = 32;
  std::cout << "checked(ok=" << ok << ")" << std::endl;
  float *p = 0;

  {
    SecureArray<float,size> secf;
    
    // for reference to data secton after secf is cleaned off of stack,
    // just to test that the memory has been cleared.
    p = &secf.data[0]; 
    for (int i=0; i<size; ++i) { assert(p[i] == float(0.0)); }
    if (!ok) {
      double *d = (double*)&secf.data;
      for (int i=0; i<size; ++i) { d[i]=sqrt(i); }
    } else {
      float *f = (float*)&secf.data;
      for (int i=0; i<size; ++i) { f[i]=sqrt(i); }
    }
  }
  // ISR could make this fail, because it would reuse the frame memory,
  // data is reset to zero when securedata is destroyed.
  for (int i=0; i<size; ++i) { assert(p[i] == float(0.0)); }
}

void safe(bool ok) {
  checked(ok);
  exit(0);
}

void monitor(pid_t child, bool ok) {
  int status;
  
  if ( waitpid(child, &status, 0) == -1 ) {
    std::cout << "wait on child failed" << std::endl;
    exit(3);
  }
  
  if ( WIFEXITED(status) ) {
    int exitStatus = WEXITSTATUS(status);
    std::cout << " child exited with status " << exitStatus << std::endl;
    
    int expect = ok ? 0 : -1;
    if (exitStatus != expect) {
      std::cout << "got exit status of " << exitStatus << ", not " << expect << std::endl;
      exit(1);
    }
  }
  
  if ( WIFSIGNALED(status) ) {
    int exitSignal = WTERMSIG(status);
    std::cout << " child exited with signal " << exitSignal << std::endl;
    
    int expect = !ok ? SIGABRT : -1;
    if (exitSignal != expect) {
      std::cout << "got exit signal of " << exitSignal << ", not " << expect << std::endl;
      exit(1);
    }
  }
  std::cout << "test passed" << std::endl;
}

void testSafe(bool ok) {
  pid_t child = fork();
  if (child == -1) {
    std::cout << "fork failed." << std::endl;
    exit(1);
  }
  if (child == 0) {
    safe(ok);
  } else {
    monitor(child,ok);
  }
}

int main(int argc, const char *argv[]) {
  { bool ok = true;  testSafe(ok); }
  { bool ok = false;  testSafe(ok); }
  return 0;
}
