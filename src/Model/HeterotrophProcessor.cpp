#include "HeterotrophProcessor.h"

#include <cmath>
#include <iostream>

#include "Heterotroph.h"
#include "Parameters.h"
#include "RandomSimple.h"
#include "Enums.h"

HeterotrophProcessor::HeterotrophProcessor()
    : sizeClassBoundaries_(Parameters::Get()->getSizeClassBoundaries()),
      linearFeedingDenominators_(Parameters::Get()->getLinearFeedingDenominators()),
      halfSaturationConstants_(Parameters::Get()->getHalfSaturationConstants()),
      preferredPreyVolumeRatio_(Parameters::Get()->getPreferredPreyVolumeRatio()),
      preferenceFunctionWidth_(Parameters::Get()->getPreferenceFunctionWidth()),
      fractionalMetabolicExpense_(Parameters::Get()->getFractionalMetabolicExpense()),
      metabolicIndex_(Parameters::Get()->getMetabolicIndex()),
      numberOfSizeClasses_(Parameters::Get()->getNumberOfSizeClasses()),
      largestVolumeExponent_(Parameters::Get()->getLargestVolumeExponent()),
      smallestVolumeExponent_(Parameters::Get()->getSmallestVolumeExponent()),
      preferenceDenominator_(2 * std::pow(preferenceFunctionWidth_, 2)) {}

double HeterotrophProcessor::calculateMetabolicDeduction(const Heterotroph& heterotroph) const {
  return fractionalMetabolicExpense_ * std::pow(heterotroph.getVolumeActual(), metabolicIndex_);
}

double HeterotrophProcessor::calculatePreferenceForPrey(const double grazerVolume, const double preyVolume) const {
  return std::exp(-std::pow((std::log((preferredPreyVolumeRatio_ * preyVolume) / grazerVolume)), 2) / preferenceDenominator_);
}

double HeterotrophProcessor::calculateFeedingProbability(const unsigned predatorIndex,
                                                         const double effectivePreyVolume) {
  return calculateFeedingProbabilityNonLinear(predatorIndex, effectivePreyVolume);
}

double HeterotrophProcessor::calculateStarvationProbability(const Heterotroph& heterotroph) const {
  return calculateLinearStarvation(heterotroph.getVolumeActual(), heterotroph.getVolumeHeritable(),
                                   heterotroph.getVolumeMinimum(), heterotroph.getStarvationMultiplier());
}

unsigned HeterotrophProcessor::findIndividualSizeClassIndex(const Heterotroph& heterotroph,
                                                            unsigned directionToMove) const {

  // PJU FIX
  unsigned currentSizeClass = 0;  // heterotroph.getSizeClassIndex();
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

bool HeterotrophProcessor::updateSizeClassIndex(Heterotroph& heterotroph) const {
  unsigned directionToMove = directionIndividualShouldMoveSizeClasses(heterotroph);
  if (directionToMove != enums::eNoMovement) {
    // PJU FIX
    // unsigned newSizeClassIndex = findIndividualSizeClassIndex(heterotroph, directionToMove);
    // heterotroph.setSizeClassIndex(newSizeClassIndex);
    return true;
  }
  return false;
}

unsigned HeterotrophProcessor::findSizeClassIndexFromVolume(const double volume) const {
  unsigned sizeClassIndex = 0;

  for (unsigned index = 1; index <= numberOfSizeClasses_; ++index) {
    if (volume < sizeClassBoundaries_[index]) {
      sizeClassIndex = index - 1;
      break;
    }
  }
  return sizeClassIndex;
}

unsigned HeterotrophProcessor::directionIndividualShouldMoveSizeClasses(const Heterotroph& heterotroph) const {
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

void HeterotrophProcessor::updateHerbivoreTrophicIndex(Heterotroph* grazer) {
  double trophicLevel = grazer->getTrophicLevel();
  if (trophicLevel != 0)
    grazer->setTrophicLevel((trophicLevel + 2) * 0.5);
  else
    grazer->setTrophicLevel(2);
}

void HeterotrophProcessor::updateCarnivoreTrophicIndex(Heterotroph* predator,
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

double HeterotrophProcessor::calculateFeedingProbabilityLinear(const unsigned predatorIndex,
                                                               const double effectivePreyVolume) {
  return std::min(effectivePreyVolume / linearFeedingDenominators_[predatorIndex], 1.0);
}

double HeterotrophProcessor::calculateFeedingProbabilityNonLinear(const unsigned predatorIndex,
                                                                  const double effectivePreyVolume) {
  return (effectivePreyVolume / (halfSaturationConstants_[predatorIndex] + effectivePreyVolume));
}

double HeterotrophProcessor::calculateLinearStarvation(const double volumeActual,
                                                       const double volumeHeritable,
                                                       const double volumeMinimum,
                                                       const double starvationMultiplier) const {
  if (volumeActual <= volumeMinimum)
    return 1;
  else if (volumeActual >= volumeHeritable)
    return 0;
  else
    return (1 + ((volumeMinimum - volumeActual) * starvationMultiplier));
}

double HeterotrophProcessor::calculateBetaExponentialStarvation(const double volumeActual,
                                                                const double volumeHeritable,
                                                                const double volumeMinimum,
                                                                const double starvationMultiplier) const {
  if (volumeActual <= volumeMinimum)
    return 1;
  else if (volumeActual >= volumeHeritable)
    return 0;
  else
    return (1 - (1 + ((volumeHeritable - volumeMinimum) - (volumeActual - volumeMinimum)) *
           starvationMultiplier) * ((volumeActual - volumeMinimum) * starvationMultiplier));
}

double HeterotrophProcessor::traitValueToVolume(const double traitValue) const {
  double volumeExponent = traitValue * (largestVolumeExponent_ - smallestVolumeExponent_) + smallestVolumeExponent_;
  return std::pow(10, volumeExponent);
}

double HeterotrophProcessor::volumeToTraitValue(const double volume) const {
  return (std::log10(volume) - smallestVolumeExponent_) / (largestVolumeExponent_ - smallestVolumeExponent_);
}

int HeterotrophProcessor::roundWithProbability(RandomSimple& random, const double value) const {
  int flooredValue = static_cast<int>(::floor(value));
  double probability = value - flooredValue;

  if (random.getUniform() < probability) {
    return flooredValue + 1;
  } else {
    return flooredValue;
  }
}
