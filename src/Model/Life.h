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

#include <stdfloat>
#include <vector>

#include "Nutrient.h"
#include "Parameters.h"
#include "RandomSimple.h"
#include "HeterotrophPopulation.h"

#include "Buffer.h"

class Life {
 public:
  Life() = delete;
  explicit Life(Nutrient&, Parameters&);

  void update();

 private:
  void moveHeterotrophs();

  Nutrient& nutrient_;
  Parameters& params_;

  Autotrophs autotrophs_;
  HeterotrophPopulation heterotrophs_;

  std::vector<MovingHeterotroph> movingHeterotrophs_;

  std::uint64_t varTotalHeterotrophFrequency_;
  std::float64_t varTotalHeterotrophVolume_;

  jino::Buffer<std::uint64_t> buffTotalHeterotrophFrequency_;
  jino::Buffer<std::float64_t> buffTotalHeterotrophVolume_;
};

#endif
