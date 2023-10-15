#ifndef STRUCTS
#define STRUCTS

#include "Heterotroph.h"

namespace structs {

struct MovingHeterotroph {
  Heterotroph& heterotroph_;
  enums::eMovementDirection direction_;
  MovingHeterotroph(Heterotroph& heterotroph, enums::eMovementDirection direction) :
    heterotroph_(heterotroph), direction_(direction) {}
};

}  // namespace structs

#endif
