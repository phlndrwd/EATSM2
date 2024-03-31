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
#include "Enums.h"
#include "Parameters.h"
#include "SizeClass.h"

class EncounterAlgorithm {
 public:
  EncounterAlgorithm(EcologicalData&, EcologicalFunctions&, Parameters&, const unsigned&);

  void update(std::vector<SizeClass>&, SizeClass&);

 private:
  std::vector<SizeClass>::iterator calcFeedingProbability(std::vector<SizeClass>&, SizeClass&);
  void calcEffectiveSizeClassVolumes(std::vector<SizeClass>&, SizeClass&, std::vector<double>&);
  std::vector<SizeClass>::iterator setCoupledSizeClass(const std::vector<double>&, std::vector<SizeClass>&);
  void calcFeedingStrategy();

  void feedFromAutotrophs(Heterotroph&);
  void feedFromHeterotrophs(Heterotroph&, std::vector<SizeClass>::iterator);

  EcologicalData& data_;
  EcologicalFunctions& functions_;
  Parameters& params_;

  RandomSimple random_;

  const std::vector<std::vector<double>> interSizeClassPreferences_;
  const std::vector<std::vector<double>> interSizeClassVolumes_;
  const unsigned numberOfSizeClasses_;

  enums::eFeedingStrategy feedingStrategy_;

  double effectivePreyVolume_;
  double effectiveAutotrophVolume_;
  double feedingProbabilty_;
};
#endif
