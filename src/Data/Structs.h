#ifndef STRUCTS
#define STRUCTS

#include "Heterotroph.h"

namespace structs {

struct MovingHeterotroph {
  Heterotroph& heterotroph;
  enums::eMovementDirection direction;
};

}  // namespace structs

#endif
