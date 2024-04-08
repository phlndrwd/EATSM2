/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef STRUCTS
#define STRUCTS

#include <cstdint>

#include "Heterotroph.h"

namespace structs {

struct MovingHeterotroph {
  Heterotroph& heterotroph;
  std::uint32_t origSizeClassIndex;
  enums::eMovementDirection direction;
  MovingHeterotroph(Heterotroph& _heterotroph, std::uint32_t _prevSizeClassIndex, enums::eMovementDirection _direction):
      heterotroph(_heterotroph), origSizeClassIndex(_prevSizeClassIndex), direction(_direction) {}
};

}  // namespace structs

#endif
