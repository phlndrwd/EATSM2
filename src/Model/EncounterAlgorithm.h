/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef ENCOUNTERALGORITHM
#define ENCOUNTERALGORITHM

#include "Enums.h"
#include "SizeClass.h"

#include <vector>

class EncounterAlgorithm {
 public:
  EncounterAlgorithm(const unsigned);

  void update(std::vector<SizeClass>&, SizeClass&);

 private:
  std::vector<SizeClass>::iterator calcFeedingProbability(std::vector<SizeClass>&, SizeClass&);
  void calcEffectiveSizeClassVolumes(std::vector<SizeClass>&, SizeClass&, std::vector<double>&);
  std::vector<SizeClass>::iterator setCoupledSizeClass(const std::vector<double>&, std::vector<SizeClass>&);
  void calcFeedingStrategy();

  double functionalResponseLinear(const unsigned, const double);
  double functionalResponseNonLinear(const unsigned, const double);

  void feedFromAutotrophs(Heterotroph&);
  void feedFromHeterotrophs(Heterotroph&, std::vector<SizeClass>::iterator);

  RandomSimple random_;
  const std::vector<std::vector<double>> interSizeClassPreferences_;
  const std::vector<std::vector<double>> interSizeClassVolumes_;
  const std::vector<double> linearFeedingDenominators_;
  const std::vector<double> halfSaturationConstants_;
  const unsigned numberOfSizeClasses_;

  enums::eFeedingStrategy feedingStrategy_;

  double effectivePreyVolume_;
  double effectiveAutotrophVolume_;
  double feedingProbabilty_;
};
#endif
