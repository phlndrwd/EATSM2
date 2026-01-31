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

#include "Functions.h"
#include "Parameters.h"
#include "HeterotrophPopulation.h"

enum eFeedingStrategy {
  eNotEating,
  eHerbivore,
  eCarnivore
};

struct PreyVolumes {
  PreyVolumes(): total(0), autotroph(0) {}
  std::float64_t total;
  std::float64_t autotroph;
};

class EncounterAlgorithm {
 public:
  EncounterAlgorithm(Autotrophs*, Nutrient*, Parameters*, const std::uint32_t&);

  void update(std::vector<HeterotrophPopulation>&);

 private:
  std::float64_t calcFeedingProbability(std::vector<HeterotrophPopulation>&, HeterotrophPopulation&,
                                        std::uint32_t&, eFeedingStrategy&);
  PreyVolumes calcEffectivePreyVolumes(std::vector<HeterotrophPopulation>&, HeterotrophPopulation&,
                                       std::vector<std::float64_t>&);
  std::uint32_t setCoupledSizeClassIndex(const std::vector<std::float64_t>&,
                                        PreyVolumes&, eFeedingStrategy&);
  void feedFromAutotrophs(Heterotroph&);
  void feedFromHeterotrophs(Heterotroph&, HeterotrophPopulation&);

  Autotrophs* autotrophs_;
  Nutrient* nutrient_;

  Functions functions_;
  RandomSimple random_;

  const std::vector<std::vector<std::float64_t>> interSizeClassPreferences_;
  const std::vector<std::vector<std::float64_t>> interSizeClassVolumes_;
  const std::uint32_t numberOfSizeClasses_;
  const std::uint32_t autotrophSizeIndex_;
  const std::float64_t autotrophCellSize_;
};
#endif
