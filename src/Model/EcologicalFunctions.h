/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef ECOLOGICALFUNCTIONS_H
#define ECOLOGICALFUNCTIONS_H

#include <functional>
#include <vector>

#include "Heterotroph.h"
#include "Parameters.h"

class EcologicalFunctions {
 public:
  EcologicalFunctions(Parameters&);

  std::float64_t functionalResponseLinear(const std::uint32_t&, const std::float64_t&) const;
  std::float64_t functionalResponseNonLinear(const std::uint32_t&, const std::float64_t&) const;

  std::float64_t calcMetabolicDeduction(const Heterotroph*) const;
  bool updateSizeClassIndex(Heterotroph*) const;
  std::uint32_t directionIndividualShouldMoveSizeClasses(const Heterotroph*) const;
  std::uint32_t findIndividualSizeClassIndex(const Heterotroph*, std::uint32_t&) const;
  std::float64_t calcStarvationProbability(const Heterotroph*) const;

  std::float64_t calcPreferenceForPrey(const std::float64_t&, const std::float64_t&) const;

  void updateHerbivoreTrophicIndex(Heterotroph*);
  void updateCarnivoreTrophicIndex(Heterotroph*, const Heterotroph*);

  std::float64_t calcHerbivoreTrophicIndex(const std::float64_t&) const;
  std::float64_t calcCarnivoreTrophicIndex(const std::float64_t&, const std::float64_t&) const;

  std::float64_t traitValueToVolume(const std::float64_t&) const;
  std::float64_t volumeToTraitValue(const std::float64_t&) const;

 private:
  void calcPreferenceMatrices(Parameters&);

  std::float64_t calcLinearStarvation(const std::float64_t&, const std::float64_t&, const std::float64_t&, const std::float64_t&) const;
  std::float64_t calcBetaExponentialStarvation(const std::float64_t&, const std::float64_t&, const std::float64_t&, const std::float64_t&) const;

  std::function<std::float64_t(const std::uint32_t, const std::float64_t)> starvationProbabilityFunc_;

  const std::vector<std::float64_t> sizeClassBoundaries_;
  const std::vector<std::float64_t> linearFeedingDenominators_;
  const std::vector<std::float64_t> halfSaturationConstants_;

  const std::float64_t largestVolumeExponent_;
  const std::float64_t smallestVolumeExponent_;

  const std::float64_t preferredPreyVolumeRatio_;
  const std::float64_t preferenceFunctionWidth_;
  const std::float64_t fractionalMetabolicExpense_;
  const std::float64_t metabolicIndex_;
  const std::float64_t numberOfSizeClasses_;

  const std::float64_t preferenceDenominator_;
};

#endif
