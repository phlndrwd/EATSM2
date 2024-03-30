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

#include "EncounterAlgorithm.h"
#include "Nutrient.h"
#include "RandomSimple.h"
#include "SizeClass.h"

class Life {
 public:
  Life() = delete;
   explicit Life(Nutrient&);

  void update();

 private:
  unsigned findSizeClassIndexFromVolume(const double) const;

  Nutrient& nutrient_;

  const std::vector<double> sizeClassBoundaries_;
  const unsigned numberOfSizeClasses_;

  RandomSimple random_;
  EncounterAlgorithm encounterAlgorithm_;
  std::vector<SizeClass> sizeClasses_;
};

#endif
