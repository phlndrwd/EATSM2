/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef LIFE_H
#define LIFE_H

#include <vector>

#include "EcologicalData.h"
#include "EcologicalFunctions.h"
#include "EncounterAlgorithm.h"
#include "Nutrient.h"
#include "Parameters.h"
#include "RandomSimple.h"
#include "SizeClass.h"

class Life {
 public:
  Life() = delete;
  explicit Life(Nutrient&, Parameters&);

  void update();

 private:
  unsigned findSizeClassIndexFromVolume(const double&) const;

  Nutrient& nutrient_;
  Parameters& params_;

  EcologicalData data_;
  EcologicalFunctions functions_;
  RandomSimple random_;
  EncounterAlgorithm algorithm_;

  std::vector<SizeClass> sizeClasses_;
  std::vector<structs::MovingHeterotroph> movingHeterotrophs_;

  unsigned numberOfSizeClasses_;
};

#endif
