/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "TimeStep.h"

TimeStep::TimeStep(const std::uint64_t& samplingRate) :
    samplingRate_(samplingRate) {
  timeStep_ = 0;
}

const std::uint64_t& TimeStep::getTimeStep() const {
  return timeStep_;
}

bool TimeStep::takeSnapshot() const {
  return (timeStep_ % samplingRate_ == 0);
}

void TimeStep::incrementTimeStep() {
  ++timeStep_;
}
