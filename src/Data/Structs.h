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
#include <memory>

#include "Heterotroph.h"

namespace structs {

struct MovingHeterotroph {
  std::shared_ptr<Heterotroph> heterotroph;
  std::uint32_t prevSizeClassIndex;
  enums::eGrowthTrajectory growthTrajectory;
  MovingHeterotroph(std::shared_ptr<Heterotroph> _heterotroph, std::uint32_t _prevSizeClassIndex, enums::eGrowthTrajectory _growthTrajectory):
      heterotroph(_heterotroph), prevSizeClassIndex(_prevSizeClassIndex), growthTrajectory(_growthTrajectory) {}
};

}  // namespace structs

#endif
