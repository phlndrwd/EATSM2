/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "EcologicalFunctions.h"

#include <cmath>

#include "Heterotroph.h"
#include "Parameters.h"
#include "Enums.h"

EcologicalFunctions::EcologicalFunctions(EcologicalData& data, Parameters& params) :
    data_(data),
    sizeClassBoundaries_(data.getSizeClassBoundaries()),
    linearFeedingDenominators_(data.getLinearFeedingDenominators()),
    halfSaturationConstants_(data.getHalfSaturationConstants()),
    largestVolumeExponent_(data.getLargestVolumeExponent()),
    smallestVolumeExponent_(data.getSmallestVolumeExponent()),
    preferredPreyVolumeRatio_(params.getPreferredPreyVolumeRatio()),
    preferenceFunctionWidth_(params.getPreferenceFunctionWidth()),
    fractionalMetabolicExpense_(params.getFractionalMetabolicExpense()),
    metabolicIndex_(params.getMetabolicIndex()),
    numberOfSizeClasses_(params.getNumberOfSizeClasses()),
    preferenceDenominator_(2 * std::pow(preferenceFunctionWidth_, 2)) {
  calcPreferenceMatrices();
}

void EcologicalFunctions::calcPreferenceMatrices() {
  const std::vector<std::float64_t>& sizeClassMidPoints = data_.getSizeClassMidPoints();
  // Calculating here to avoid circular dependeny in EcologicalData
  std::vector<std::vector<std::float64_t>>& interSizeClassPreferences = data_.getInterSizeClassPreferences();
  std::vector<std::vector<std::float64_t>>& interSizeClassVolumes = data_.getInterSizeClassVolumes();

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

std::float64_t EcologicalFunctions::functionalResponseLinear(const std::uint32_t& predatorIndex, const std::float64_t& effectivePreyVolume) const {
  return static_cast<std::float64_t>(std::min(static_cast<double>(effectivePreyVolume) / static_cast<double>(linearFeedingDenominators_[predatorIndex]), 1.0));
}

std::float64_t EcologicalFunctions::functionalResponseNonLinear(const std::uint32_t& predatorIndex, const std::float64_t& effectivePreyVolume) const {
  return (effectivePreyVolume / (halfSaturationConstants_[predatorIndex] + effectivePreyVolume));
}


std::float64_t EcologicalFunctions::calcMetabolicDeduction(const Heterotroph& heterotroph) const {
  return fractionalMetabolicExpense_ * std::pow(heterotroph.getVolumeActual(), metabolicIndex_);
}

std::float64_t EcologicalFunctions::calcPreferenceForPrey(const std::float64_t& grazerVolume, const std::float64_t& preyVolume) const {
  return std::exp(-std::pow((std::log((preferredPreyVolumeRatio_ * preyVolume) / grazerVolume)), 2) / preferenceDenominator_);
}

std::float64_t EcologicalFunctions::calcStarvationProbability(const Heterotroph& heterotroph) const {
  return calcLinearStarvation(heterotroph.getVolumeActual(), heterotroph.getVolumeHeritable(),
                              heterotroph.getVolumeMinimum(), heterotroph.getStarvationMultiplier());
}

std::uint32_t EcologicalFunctions::findIndividualSizeClassIndex(const Heterotroph& heterotroph,
                                                            std::uint32_t& directionToMove) const {
  std::uint32_t currentSizeClass = 0; // PJU FIX - heterotroph.getSizeClassIndex();
  std::uint32_t newSizeClassIndex = currentSizeClass;
  std::float64_t volume = heterotroph.getVolumeActual();

  if (directionToMove == enums::eGrowing) {
    for (std::uint32_t index = currentSizeClass; index < numberOfSizeClasses_; ++index) {
      if (volume < sizeClassBoundaries_[index]) {
        newSizeClassIndex = index - 1;
        break;
      }
    }
  } else if (directionToMove == enums::eShrinking) {
    for (std::int32_t index = currentSizeClass; index >= 0; --index) {
      if (volume >= sizeClassBoundaries_[index]) {
        newSizeClassIndex = (std::uint32_t)index;
        break;
      }
    }
  }

  return newSizeClassIndex;
}

bool EcologicalFunctions::updateSizeClassIndex(Heterotroph& heterotroph) const {
  std::uint32_t directionToMove = directionIndividualShouldMoveSizeClasses(heterotroph);
  if (directionToMove != enums::eStatic) {
    // PJU FIX
    // std::uint32_t newSizeClassIndex = findIndividualSizeClassIndex(heterotroph, directionToMove);
    // heterotroph.setSizeClassIndex(newSizeClassIndex);
    return true;
  }
  return false;
}

std::uint32_t EcologicalFunctions::directionIndividualShouldMoveSizeClasses(const Heterotroph& heterotroph) const {
  std::uint32_t directionToMove = enums::eStatic;

  // PJU FIX
  std::uint32_t sizeClassIndex = 0;  // heterotroph.getSizeClassIndex();
  std::float64_t volumeActual = heterotroph.getVolumeActual();

  if (volumeActual < sizeClassBoundaries_[sizeClassIndex])
    directionToMove = enums::eShrinking;
  else if (volumeActual >= sizeClassBoundaries_[sizeClassIndex + 1])
    directionToMove = enums::eGrowing;

  return directionToMove;
}

void EcologicalFunctions::updateHerbivoreTrophicIndex(Heterotroph* grazer) {
  std::float64_t trophicLevel = grazer->getTrophicLevel();
  if (trophicLevel != 0)
    grazer->setTrophicLevel((trophicLevel + 2) * 0.5);
  else
    grazer->setTrophicLevel(2);
}

void EcologicalFunctions::updateCarnivoreTrophicIndex(Heterotroph* predator,
                                                       const Heterotroph* prey) {
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

//std::float64_t EcologicalFunctions::calcFeedingProbabilityLinear(const std::uint32_t predatorIndex,
//                                                          const std::float64_t effectivePreyVolume) {
//  return std::min(effectivePreyVolume / linearFeedingDenominators_[predatorIndex], 1.0);
//}

//std::float64_t EcologicalFunctions::calcFeedingProbabilityNonLinear(const std::uint32_t predatorIndex,
//                                                             const std::float64_t effectivePreyVolume) {
//  return (effectivePreyVolume / (halfSaturationConstants_[predatorIndex] + effectivePreyVolume));
//}

std::float64_t EcologicalFunctions::calcLinearStarvation(const std::float64_t& volumeActual,
                                                 const std::float64_t& volumeHeritable,
                                                 const std::float64_t& volumeMinimum,
                                                 const std::float64_t& starvationMultiplier) const {
  if (volumeActual <= volumeMinimum)
    return 1;
  else if (volumeActual >= volumeHeritable)
    return 0;
  else
    return (1 + ((volumeMinimum - volumeActual) * starvationMultiplier));
}

std::float64_t EcologicalFunctions::calcBetaExponentialStarvation(const std::float64_t& volumeActual,
                                                          const std::float64_t& volumeHeritable,
                                                          const std::float64_t& volumeMinimum,
                                                          const std::float64_t& starvationMultiplier) const {
  if (volumeActual <= volumeMinimum)
    return 1;
  else if (volumeActual >= volumeHeritable)
    return 0;
  else
    return (1 - (1 + ((volumeHeritable - volumeMinimum) - (volumeActual - volumeMinimum)) *
           starvationMultiplier) * ((volumeActual - volumeMinimum) * starvationMultiplier));
}

std::float64_t EcologicalFunctions::traitValueToVolume(const std::float64_t& traitValue) const {
  std::float64_t volumeExponent = traitValue * (largestVolumeExponent_ - smallestVolumeExponent_) + smallestVolumeExponent_;
  return std::pow(10, volumeExponent);
}

std::float64_t EcologicalFunctions::volumeToTraitValue(const std::float64_t& volume) const {
  return (std::log10(volume) - smallestVolumeExponent_) / (largestVolumeExponent_ - smallestVolumeExponent_);
}
