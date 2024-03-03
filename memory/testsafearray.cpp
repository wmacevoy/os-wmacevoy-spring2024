#include <iostream>

#include <unistd.h>
#include "securearray.h"

void testCanary(int b, int e) {
    SecureArray<int,10> buffer;
    for (int i=b; i<e; ++i) {
      buffer.data[i]=i;
    }
}

// errors segfault so tests must be run as a child process
void testCanaryAsChild(int b, int e, int expected) {
  pid_t child;
  if ((child=fork()) == 0) {
    testCanary(b,e);
  } else {
     int status;

     if ( waitpid(child, &status, 0) == -1 ) {
       perror("waitpid() failed");
       exit(EXIT_FAILURE);
     }
     
     if ( WIFEXITED(status) ) {
       int es = WEXITSTATUS(status);
       printf("testCanary(%d,%d) exit status is %d\n",b,e,es);
       if (es != expected) {
	 printf("expected exit status %d but got %d\n", expected, es);
	 exit(EXIT_FAILURE);
       }
     }
  }
}

int main(int argc, const char *argv[]) {
  testCanaryAsChild(0,10,0);
  testCanaryAsChild(-1,10,1);
  testCanaryAsChild(0,11,1);
}
