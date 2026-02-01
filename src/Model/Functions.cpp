/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Functions.h"

#include <cmath>

#include "Heterotroph.h"
#include "Parameters.h"

Functions::Functions(Parameters& params) :
    sizeClassBoundaries_(params.getSizeClassBoundaries()),
    linearFeedingDenominators_(params.getLinearFeedingDenominators()),
    halfSaturationConstants_(params.getHalfSaturationConstants()),
    largestVolumeExponent_(params.getLargestVolumeExponent()),
    smallestVolumeExponent_(params.getSmallestVolumeExponent()),
    preferredPreyVolumeRatio_(params.getPreferredPreyVolumeRatio()),
    preferenceFunctionWidth_(params.getPreferenceFunctionWidth()),
    fractionalMetabolicExpense_(params.getFractionalMetabolicExpense()),
    metabolicIndex_(params.getMetabolicIndex()),
    numberOfSizeClasses_(params.getNumberOfSizeClasses()) {
  calcPreferenceMatrices(params);
}

std::float64_t Functions::functionalResponse(const std::uint32_t& predatorIndex, const std::float64_t& effectivePreyVolume) const {
  return (effectivePreyVolume / (halfSaturationConstants_[predatorIndex] + effectivePreyVolume));
}

std::float64_t Functions::calcMetabolicDeduction(const Heterotroph& heterotroph) const {
  return fractionalMetabolicExpense_ * std::pow(heterotroph.getVolumeActual(), metabolicIndex_);
}

std::float64_t Functions::traitValueToVolume(const std::float64_t& traitValue, const std::float64_t& smallestVolumeExponent, const std::float64_t& largestVolumeExponent) {
  std::float64_t volumeExponent = traitValue * (largestVolumeExponent - smallestVolumeExponent) + smallestVolumeExponent;
  return std::pow(10, volumeExponent);
}

std::float64_t Functions::calcStarvationProbability(const Heterotroph& heterotroph) const {
  const std::float64_t& volumeActual = heterotroph.getVolumeActual();
  const std::float64_t& volumeMinimum = heterotroph.getVolumeMinimum();

  if (volumeActual <= volumeMinimum)
    return 1;
  else if (volumeActual >= heterotroph.getVolumeHeritable())
    return 0;
  else
    return (1 + ((volumeMinimum - volumeActual) * heterotroph.getStarvationMultiplier()));
}
