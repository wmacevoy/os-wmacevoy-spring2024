#include "canary.h"
#include "randomize.h"

const Canary::Type Canary::reference = randomize<Canary::Type>();
