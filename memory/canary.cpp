#include "canary.h"
#include "randomize.h"

//
// This value is initialized once per process.
// Child processes without exec() will have the same canary value by necessity.
//
const Canary::Type Canary::reference = randomize<Canary::Type>();

