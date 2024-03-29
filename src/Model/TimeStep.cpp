/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "TimeStep.h"
#include "Parameters.h"

TimeStep::TimeStep() :
    samplingRate_(Parameters::Get()->getSamplingRate()) {
  timeStep_ = 0;
}

const unsigned& TimeStep::getTimeStep() const {
  return timeStep_;
}

bool TimeStep::doRecordData() const {
  return (timeStep_ % samplingRate_ == 0);
}

void TimeStep::incrementTimeStep() {
  ++timeStep_;
}
