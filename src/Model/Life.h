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

#include "EncounterAlgorithm.h"
#include "Nutrient.h"
#include "Parameters.h"
#include "RandomSimple.h"
#include "SizeClass.h"

#include "Buffer.h"

class Life {
 public:
  Life() = delete;
  explicit Life(Nutrient&, Parameters&);

  void update();

 private:
  void moveHeterotrophs();
  std::uint32_t findSizeClassIndexFromVolume(const std::float64_t&) const;

  Nutrient& nutrient_;
  Parameters& params_;

  RandomSimple random_;
  EncounterAlgorithm algorithm_;

  std::vector<SizeClass> sizeClasses_;
  std::vector<structs::MovingHeterotroph> movingHeterotrophs_;

  std::uint32_t numberOfSizeClasses_;

  std::uint64_t varTotalHeterotrophFrequency_;
  std::float64_t varTotalHeterotrophVolume_;
  std::float64_t varTotalAutotrophVolume_;

  jino::Buffer<std::uint64_t> buffTotalHeterotrophFrequency_;
  jino::Buffer<std::float64_t> buffTotalHeterotrophVolume_;
  jino::Buffer<std::float64_t> buffTotalAutotrophVolume_;
};

#endif
