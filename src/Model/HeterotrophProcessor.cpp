#include "HeterotrophProcessor.h"

#include <cmath>
#include <iostream>

#include "Heterotroph.h"
#include "Parameters.h"
#include "RandomSimple.h"

HeterotrophProcessor::HeterotrophProcessor()
    : mSizeClassBoundaries(Parameters::Get()->GetSizeClassBoundaries()),
      mLinearFeedingDenominators(Parameters::Get()->GetLinearFeedingDenominators()),
      mHalfSaturationConstants(Parameters::Get()->GetHalfSaturationConstants()),
      mPreferredPreyVolumeRatio(Parameters::Get()->GetPreferredPreyVolumeRatio()),
      mPreferenceFunctionWidth(Parameters::Get()->GetPreferenceFunctionWidth()),
      mFractionalMetabolicExpense(Parameters::Get()->GetFractionalMetabolicExpense()),
      mMetabolicIndex(Parameters::Get()->GetMetabolicIndex()),
      mNumberOfSizeClasses(Parameters::Get()->GetNumberOfSizeClasses()),
      mLargestVolumeExponent(Parameters::Get()->GetLargestVolumeExponent()),
      mSmallestVolumeExponent(Parameters::Get()->GetSmallestVolumeExponent()),
      mPreferenceDenominator(2 * std::pow(mPreferenceFunctionWidth, 2)) {
  // Set function option once for the duration of the model execution
  if (Parameters::Get()->GetUseLinearFeeding() == true)
    fStarvationProbability = &HeterotrophProcessor::CalculateFeedingProbabilityLinear;
  else
    fStarvationProbability = &HeterotrophProcessor::CalculateFeedingProbabilityNonLinear;
}

HeterotrophProcessor::~HeterotrophProcessor() {}

double HeterotrophProcessor::CalculatePreferenceForPrey(const double grazerVolume, const double preyVolume) const {
  return std::exp(-std::pow((std::log((mPreferredPreyVolumeRatio * preyVolume) / grazerVolume)), 2) /
                  mPreferenceDenominator);
}

double HeterotrophProcessor::CalculateFeedingProbability(const unsigned predatorIndex,
                                                         const double effectivePreyVolume) {
  return (this->*fStarvationProbability)(predatorIndex, effectivePreyVolume);
}

double HeterotrophProcessor::CalculateMetabolicDeduction(const Heterotroph* individual) const {
  return mFractionalMetabolicExpense * std::pow(individual->GetVolumeActual(), mMetabolicIndex);
}

double HeterotrophProcessor::CalculateStarvationProbability(const Heterotroph* individual) const {
  return CalculateLinearStarvation(individual->GetVolumeActual(), individual->GetVolumeHeritable(),
                                   individual->GetVolumeMinimum(), individual->GetStarvationMultiplier());
}

unsigned HeterotrophProcessor::FindIndividualSizeClassIndex(const Heterotroph* individual,
                                                            unsigned directionToMove) const {
  unsigned currentSizeClass = individual->GetSizeClassIndex();
  unsigned newSizeClassIndex = currentSizeClass;
  double volume = individual->GetVolumeActual();

  if (directionToMove == Constants::eMoveUp) {
    for (unsigned index = currentSizeClass; index < mNumberOfSizeClasses; ++index) {
      if (volume < mSizeClassBoundaries[index]) {
        newSizeClassIndex = index - 1;
        break;
      }
    }
  } else if (directionToMove == Constants::eMoveDown) {
    for (int index = currentSizeClass; index >= 0; --index) {
      if (volume >= mSizeClassBoundaries[index]) {
        newSizeClassIndex = (unsigned)index;
        break;
      }
    }
  }

  return newSizeClassIndex;
}

bool HeterotrophProcessor::UpdateSizeClassIndex(Heterotroph* individual) const {
  unsigned directionToMove = DirectionIndividualShouldMoveSizeClasses(individual);
  if (directionToMove != Constants::eNoMovement) {
    unsigned newSizeClassIndex = FindIndividualSizeClassIndex(individual, directionToMove);
    individual->SetSizeClassIndex(newSizeClassIndex);
    return true;
  }
  return false;
}

unsigned HeterotrophProcessor::FindSizeClassIndexFromVolume(const double volume) const {
  unsigned sizeClassIndex = 0;

  for (unsigned index = 1; index <= mNumberOfSizeClasses; ++index) {
    if (volume < mSizeClassBoundaries[index]) {
      sizeClassIndex = index - 1;
      break;
    }
  }
  return sizeClassIndex;
}

unsigned HeterotrophProcessor::DirectionIndividualShouldMoveSizeClasses(
    const Heterotroph* individual) const {
  unsigned directionToMove = Constants::eNoMovement;

  unsigned sizeClassIndex = individual->GetSizeClassIndex();
  double volumeActual = individual->GetVolumeActual();

  if (volumeActual < mSizeClassBoundaries[sizeClassIndex])
    directionToMove = Constants::eMoveDown;
  else if (volumeActual >= mSizeClassBoundaries[sizeClassIndex + 1])
    directionToMove = Constants::eMoveUp;

  return directionToMove;
}

void HeterotrophProcessor::UpdateHerbivoreTrophicIndex(Heterotroph* grazer) {
  double trophicLevel = grazer->GetTrophicLevel();
  if (trophicLevel != 0)
    grazer->SetTrophicLevel((trophicLevel + 2) * 0.5);
  else
    grazer->SetTrophicLevel(2);
}

void HeterotrophProcessor::UpdateCarnivoreTrophicIndex(Heterotroph* predator,
                                                       const Heterotroph* prey) {
  double predatorTrophicLevel = predator->GetTrophicLevel();
  double preyTrophicLevel = prey->GetTrophicLevel();
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
  predator->SetTrophicLevel(predatorTrophicLevel);
}

double HeterotrophProcessor::CalculateFeedingProbabilityLinear(const unsigned predatorIndex,
                                                               const double effectivePreyVolume) {
  return std::min(effectivePreyVolume / mLinearFeedingDenominators[predatorIndex], 1.0);
}

double HeterotrophProcessor::CalculateFeedingProbabilityNonLinear(const unsigned predatorIndex,
                                                                  const double effectivePreyVolume) {
  return (effectivePreyVolume / (mHalfSaturationConstants[predatorIndex] + effectivePreyVolume));
}

double HeterotrophProcessor::CalculateLinearStarvation(const double volumeActual, const double volumeHeritable,
                                                       const double volumeMinimum,
                                                       const double starvationMultiplier) const {
  if (volumeActual <= volumeMinimum)
    return 1;
  else if (volumeActual >= volumeHeritable)
    return 0;
  else
    return (1 + ((volumeMinimum - volumeActual) * starvationMultiplier));
}

double HeterotrophProcessor::CalculateBetaExponentialStarvation(const double volumeActual, const double volumeHeritable,
                                                                const double volumeMinimum,
                                                                const double starvationMultiplier) const {
  if (volumeActual <= volumeMinimum)
    return 1;
  else if (volumeActual >= volumeHeritable)
    return 0;
  else
    return (1 - (1 + ((volumeHeritable - volumeMinimum) - (volumeActual - volumeMinimum)) * starvationMultiplier) *
                    ((volumeActual - volumeMinimum) * starvationMultiplier));
}

double HeterotrophProcessor::TraitValueToVolume(const double traitValue) {
  double volumeExponent = traitValue * (mLargestVolumeExponent - mSmallestVolumeExponent) + mSmallestVolumeExponent;
  return std::pow(10, volumeExponent);
}

double HeterotrophProcessor::VolumeToTraitValue(const double volume) const {
  return (std::log10(volume) - mSmallestVolumeExponent) / (mLargestVolumeExponent - mSmallestVolumeExponent);
}

int HeterotrophProcessor::RoundWithProbability(RandomSimple& random, const double value) const {
  int flooredValue = static_cast<int>(::floor(value));
  double probability = value - flooredValue;

  if (random.GetUniform() < probability)
    return flooredValue + 1;
  else
    return flooredValue;
}
