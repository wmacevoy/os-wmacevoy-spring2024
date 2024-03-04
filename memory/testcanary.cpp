#include <string.h>
#include <iostream>

#include "randomize.h"
#include "canary.h"

void checked(bool ok) {
  Canary canary;
  if (!ok) {
    randomize(canary);
  }
}

int main(int argc, const char *argv[]) {
  bool ok = (argc > 1 && strcmp(argv[1],"--ok=true") == 0);
  std::cout << argv[0] << " " << "--ok=" << (ok ? "true" : "false") << std::endl;
  checked(ok);
  return 0;
}
