/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef ENCOUNTERALGORITHM_H
#define ENCOUNTERALGORITHM_H

#include <vector>

#include "EcologicalData.h"
#include "EcologicalFunctions.h"
#include "Parameters.h"
#include "SizeClass.h"

struct PreyVolumes {
  PreyVolumes(): totalPrey(0), autotroph(0) {}
  std::float64_t totalPrey;
  std::float64_t autotroph;
};

class EncounterAlgorithm {
 public:
  EncounterAlgorithm(Nutrient&, EcologicalData&, Parameters&, const std::uint32_t&);

  void update(std::vector<SizeClass>&, SizeClass&);

 private:
  std::float64_t calcFeedingProbability(std::vector<SizeClass>&, SizeClass&,
                                std::vector<SizeClass>::iterator&, enums::eFeedingStrategy&);
  PreyVolumes calcEffectiveSizeClassVolumes(std::vector<SizeClass>&, SizeClass&, std::vector<std::float64_t>&);
  std::vector<SizeClass>::iterator setCoupledSizeClass(const std::vector<std::float64_t>&, std::vector<SizeClass>&,
                                                       PreyVolumes&, enums::eFeedingStrategy&);
  void feedFromAutotrophs(Heterotroph&, std::vector<SizeClass>::iterator);
  void feedFromHeterotrophs(Heterotroph&, std::vector<SizeClass>::iterator);

  Nutrient& nutrient_;
  EcologicalData& data_;

  EcologicalFunctions functions_;
  RandomSimple random_;

  const std::vector<std::vector<std::float64_t>> interSizeClassPreferences_;
  const std::vector<std::vector<std::float64_t>> interSizeClassVolumes_;
  const std::uint32_t numberOfSizeClasses_;
  const std::float64_t autotrophCellSize_;
};
#endif
