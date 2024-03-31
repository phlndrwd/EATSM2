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
#include "RandomSimple.h"
#include "Enums.h"

EcologicalFunctions::EcologicalFunctions(EcologicalData& data, Parameters& params) :
    data_(data),
    params_(params),
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
    preferenceDenominator_(2 * std::pow(preferenceFunctionWidth_, 2)) {}

double EcologicalFunctions::functionalResponseLinear(const unsigned& predatorIndex, const double& effectivePreyVolume) const {
  return std::min(effectivePreyVolume / linearFeedingDenominators_[predatorIndex], 1.0);
}

double EcologicalFunctions::functionalResponseNonLinear(const unsigned& predatorIndex, const double& effectivePreyVolume) const {
  return (effectivePreyVolume / (halfSaturationConstants_[predatorIndex] + effectivePreyVolume));
}


double EcologicalFunctions::calculateMetabolicDeduction(const Heterotroph& heterotroph) const {
  return fractionalMetabolicExpense_ * std::pow(heterotroph.getVolumeActual(), metabolicIndex_);
}

double EcologicalFunctions::calculatePreferenceForPrey(const double& grazerVolume, const double& preyVolume) const {
  return std::exp(-std::pow((std::log((preferredPreyVolumeRatio_ * preyVolume) / grazerVolume)), 2) / preferenceDenominator_);
}

double EcologicalFunctions::calculateStarvationProbability(const Heterotroph& heterotroph) const {
  return calcLinearStarvation(heterotroph.getVolumeActual(), heterotroph.getVolumeHeritable(),
                                   heterotroph.getVolumeMinimum(), heterotroph.getStarvationMultiplier());
}

unsigned EcologicalFunctions::findIndividualSizeClassIndex(const Heterotroph& heterotroph,
                                                            unsigned& directionToMove) const {
  unsigned currentSizeClass = 0; // PJU FIX - heterotroph.getSizeClassIndex();
  unsigned newSizeClassIndex = currentSizeClass;
  double volume = heterotroph.getVolumeActual();

  if (directionToMove == enums::eMoveUp) {
    for (unsigned index = currentSizeClass; index < numberOfSizeClasses_; ++index) {
      if (volume < sizeClassBoundaries_[index]) {
        newSizeClassIndex = index - 1;
        break;
      }
    }
  } else if (directionToMove == enums::eMoveDown) {
    for (int index = currentSizeClass; index >= 0; --index) {
      if (volume >= sizeClassBoundaries_[index]) {
        newSizeClassIndex = (unsigned)index;
        break;
      }
    }
  }

  return newSizeClassIndex;
}

bool EcologicalFunctions::updateSizeClassIndex(Heterotroph& heterotroph) const {
  unsigned directionToMove = directionIndividualShouldMoveSizeClasses(heterotroph);
  if (directionToMove != enums::eNoMovement) {
    // PJU FIX
    // unsigned newSizeClassIndex = findIndividualSizeClassIndex(heterotroph, directionToMove);
    // heterotroph.setSizeClassIndex(newSizeClassIndex);
    return true;
  }
  return false;
}

unsigned EcologicalFunctions::directionIndividualShouldMoveSizeClasses(const Heterotroph& heterotroph) const {
  unsigned directionToMove = enums::eNoMovement;

  // PJU FIX
  unsigned sizeClassIndex = 0;  // heterotroph.getSizeClassIndex();
  double volumeActual = heterotroph.getVolumeActual();

  if (volumeActual < sizeClassBoundaries_[sizeClassIndex])
    directionToMove = enums::eMoveDown;
  else if (volumeActual >= sizeClassBoundaries_[sizeClassIndex + 1])
    directionToMove = enums::eMoveUp;

  return directionToMove;
}

void EcologicalFunctions::updateHerbivoreTrophicIndex(Heterotroph* grazer) {
  double trophicLevel = grazer->getTrophicLevel();
  if (trophicLevel != 0)
    grazer->setTrophicLevel((trophicLevel + 2) * 0.5);
  else
    grazer->setTrophicLevel(2);
}

void EcologicalFunctions::updateCarnivoreTrophicIndex(Heterotroph* predator,
                                                       const Heterotroph* prey) {
  double predatorTrophicLevel = predator->getTrophicLevel();
  double preyTrophicLevel = prey->getTrophicLevel();
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

//double EcologicalFunctions::calcFeedingProbabilityLinear(const unsigned predatorIndex,
//                                                          const double effectivePreyVolume) {
//  return std::min(effectivePreyVolume / linearFeedingDenominators_[predatorIndex], 1.0);
//}

//double EcologicalFunctions::calcFeedingProbabilityNonLinear(const unsigned predatorIndex,
//                                                             const double effectivePreyVolume) {
//  return (effectivePreyVolume / (halfSaturationConstants_[predatorIndex] + effectivePreyVolume));
//}

double EcologicalFunctions::calcLinearStarvation(const double& volumeActual,
                                                 const double& volumeHeritable,
                                                 const double& volumeMinimum,
                                                 const double& starvationMultiplier) const {
  if (volumeActual <= volumeMinimum)
    return 1;
  else if (volumeActual >= volumeHeritable)
    return 0;
  else
    return (1 + ((volumeMinimum - volumeActual) * starvationMultiplier));
}

double EcologicalFunctions::calcBetaExponentialStarvation(const double& volumeActual,
                                                          const double& volumeHeritable,
                                                          const double& volumeMinimum,
                                                          const double& starvationMultiplier) const {
  if (volumeActual <= volumeMinimum)
    return 1;
  else if (volumeActual >= volumeHeritable)
    return 0;
  else
    return (1 - (1 + ((volumeHeritable - volumeMinimum) - (volumeActual - volumeMinimum)) *
           starvationMultiplier) * ((volumeActual - volumeMinimum) * starvationMultiplier));
}

double EcologicalFunctions::traitValueToVolume(const double& traitValue) const {
  double volumeExponent = traitValue * (largestVolumeExponent_ - smallestVolumeExponent_) + smallestVolumeExponent_;
  return std::pow(10, volumeExponent);
}

double EcologicalFunctions::volumeToTraitValue(const double& volume) const {
  return (std::log10(volume) - smallestVolumeExponent_) / (largestVolumeExponent_ - smallestVolumeExponent_);
}
