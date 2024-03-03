#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "randomize.h"
#include "canary.h"

void checked(bool ok) {
  std::cout << "checked(ok=" << ok << ")" << std::endl;
  
  Canary canary;
  if (!ok) {
    randomize(canary);
  }
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
  { bool ok = false; testSafe(ok); }
  
  return 0;
}
