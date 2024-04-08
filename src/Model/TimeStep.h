/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <cstdint>

class TimeStep {
 public:
  TimeStep(const std::uint32_t&);

  const std::uint32_t& getTimeStep() const;

  bool doRecordData() const;
  void incrementTimeStep();

 private:
  const std::uint32_t samplingRate_;

  std::uint32_t timeStep_;
};

#endif
