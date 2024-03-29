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
