/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <functional>
#include <vector>

#include "Heterotroph.h"
#include "Parameters.h"

class Functions {
 public:
  Functions(Parameters&);

  std::float64_t functionalResponse(const std::uint32_t&, const std::float64_t&) const;

  std::float64_t calcMetabolicDeduction(const Heterotroph&) const;
  std::float64_t calcStarvationProbability(const Heterotroph&) const;
  std::float64_t calcPreferenceForPrey(const std::float64_t&, const std::float64_t&) const;

 private:
  void calcPreferenceMatrices(Parameters&);

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
