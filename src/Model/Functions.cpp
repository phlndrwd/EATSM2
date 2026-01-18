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
#include "Enums.h"

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
    numberOfSizeClasses_(params.getNumberOfSizeClasses()),
    preferenceDenominator_(2 * std::pow(preferenceFunctionWidth_, 2)) {
  calcPreferenceMatrices(params);
}

void Functions::calcPreferenceMatrices(Parameters& params) {
  const std::vector<std::float64_t>& sizeClassMidPoints = params.getSizeClassMidPoints();
  // Calculating here to avoid circular dependeny in EcologicalData
  std::vector<std::vector<std::float64_t>>& interSizeClassPreferences = params.getInterSizeClassPreferences();
  std::vector<std::vector<std::float64_t>>& interSizeClassVolumes = params.getInterSizeClassVolumes();

  interSizeClassPreferences.resize(numberOfSizeClasses_);
  interSizeClassVolumes.resize(numberOfSizeClasses_);

  for (std::uint32_t subjectIndex = 0; subjectIndex < numberOfSizeClasses_; ++subjectIndex) {
    std::float64_t subjectVolumeMean = sizeClassMidPoints[subjectIndex];
    std::float64_t preferenceSum = 0;

    for (std::uint32_t referenceIndex = 0; referenceIndex < numberOfSizeClasses_; ++referenceIndex) {
      std::float64_t referenceVolumeMean = sizeClassMidPoints[referenceIndex];
      std::float64_t preferenceForReferenceSizeClass = calcPreferenceForPrey(subjectVolumeMean, referenceVolumeMean);

      preferenceSum += preferenceForReferenceSizeClass;
      interSizeClassPreferences[subjectIndex].push_back(preferenceForReferenceSizeClass);
      interSizeClassVolumes[subjectIndex].push_back(preferenceForReferenceSizeClass * referenceVolumeMean);
    }
  }
}

std::float64_t Functions::functionalResponse(const std::uint32_t& predatorIndex, const std::float64_t& effectivePreyVolume) const {
  return (effectivePreyVolume / (halfSaturationConstants_[predatorIndex] + effectivePreyVolume));
}

std::float64_t Functions::calcMetabolicDeduction(const Heterotroph* heterotroph) const {
  return fractionalMetabolicExpense_ * std::pow(heterotroph->getVolumeActual(), metabolicIndex_);
}

std::float64_t Functions::calcPreferenceForPrey(const std::float64_t& grazerVolume, const std::float64_t& preyVolume) const {
  return std::exp(-std::pow((std::log((preferredPreyVolumeRatio_ * preyVolume) / grazerVolume)), 2) / preferenceDenominator_);
}

std::float64_t Functions::calcStarvationProbability(const Heterotroph* heterotroph) const {
  const std::float64_t& volumeActual = heterotroph->getVolumeActual();
  const std::float64_t& volumeMinimum = heterotroph->getVolumeMinimum();

  if (volumeActual <= volumeMinimum)
    return 1;
  else if (volumeActual >= heterotroph->getVolumeHeritable())
    return 0;
  else
    return (1 + ((volumeMinimum - volumeActual) * heterotroph->getStarvationMultiplier()));
}

void Functions::updateHerbivoreTrophicIndex(Heterotroph* grazer) {
  std::float64_t trophicLevel = grazer->getTrophicLevel();
  if (trophicLevel != 0)
    grazer->setTrophicLevel((trophicLevel + 2) * 0.5);
  else
    grazer->setTrophicLevel(2);
}

void Functions::updateCarnivoreTrophicIndex(Heterotroph* predator, const Heterotroph* prey) {
  std::float64_t predatorTrophicLevel = predator->getTrophicLevel();
  std::float64_t preyTrophicLevel = prey->getTrophicLevel();
  if (predatorTrophicLevel != 0) {
    if (preyTrophicLevel != 0)
      predatorTrophicLevel = (predatorTrophicLevel + preyTrophicLevel + 1) * 0.5;
    else
      predatorTrophicLevel = (predatorTrophicLevel + 3) * 0.5;
  } else {
    if (preyTrophicLevel != 0)
      predatorTrophicLevel = preyTrophicLevel + 1;
    else
      predatorTrophicLevel = 3;
  }
  predator->setTrophicLevel(predatorTrophicLevel);
}
