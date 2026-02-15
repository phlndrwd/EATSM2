/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Parameters.h"

#include <cmath>

#include "Constants.h"

namespace {
std::uint32_t calcDataSize(const std::uint32_t maxTimeStep, const std::uint32_t samplingRate) {
  if (maxTimeStep == 0) {
      throw std::invalid_argument("Numerator must be greater than zero...");
  }
  if (samplingRate == 0) {
      throw std::invalid_argument("Division by zero is not allowed...");
  }
  std::float64_t result = static_cast<std::float64_t>(maxTimeStep) /
                          static_cast<std::float64_t>(samplingRate);
  return static_cast<std::uint32_t>(std::ceil(result));
}
}

Parameters::Parameters(jino::Data& input):
    randomSeed_(input.getValue<std::uint8_t>(consts::kParamNames.at(eRandomSeed))),
    maxTimeStep_(input.getValue<std::uint32_t>(consts::kParamNames.at(eMaxTimeStep))),
    samplingRate_(input.getValue<std::uint32_t>(consts::kParamNames.at(eSamplingRate))),
    numberOfSizeClasses_(input.getValue<std::uint32_t>(consts::kParamNames.at(eNumberOfSizeClasses))),

    readModelState_(input.getValue<std::uint8_t>(consts::kParamNames.at(eReadModelState))),
    writeModelState_(input.getValue<std::uint8_t>(consts::kParamNames.at(eWriteModelState))),
    useLinearFeeding_(input.getValue<std::uint8_t>(consts::kParamNames.at(eUseLinearFeeding))),

    initialAutotrophicVolume_(input.getValue<std::float64_t>(consts::kParamNames.at(eInitialAutotrophicVolume))),
    initialHeterotrophicVolume_(input.getValue<std::float64_t>(consts::kParamNames.at(eInitialHeterotrophicVolume))),
    minimumHeterotrophicVolume_(input.getValue<std::float64_t>(consts::kParamNames.at(eMinimumHeterotrophicVolume))),

    smallestIndividualVolume_(input.getValue<std::float64_t>(consts::kParamNames.at(eSmallestIndividualVolume))),
    largestIndividualVolume_(input.getValue<std::float64_t>(consts::kParamNames.at(eLargestIndividualVolume))),
    sizeClassSubsetFraction_(input.getValue<std::float64_t>(consts::kParamNames.at(eSizeClassSubsetFraction))),
    halfSaturationConstantFraction_(input.getValue<std::float64_t>(consts::kParamNames.at(eHalfSaturationConstantFraction))),

    preferredPreyVolumeRatio_(input.getValue<std::uint32_t>(consts::kParamNames.at(ePreferredPreyVolumeRatio))),
    preferenceFunctionWidth_(input.getValue<std::float64_t>(consts::kParamNames.at(ePreferenceFunctionWidth))),

    assimilationEfficiency_(input.getValue<std::float64_t>(consts::kParamNames.at(eAssimilationEfficiency))),
    fractionalMetabolicExpense_(input.getValue<std::float64_t>(consts::kParamNames.at(eFractionalMetabolicExpense))),
    metabolicIndex_(input.getValue<std::float64_t>(consts::kParamNames.at(eMetabolicIndex))),

    mutationProbability_(input.getValue<std::float64_t>(consts::kParamNames.at(eMutationProbability))),
    mutationStandardDeviation_(input.getValue<std::float64_t>(consts::kParamNames.at(eMutationStandardDeviation)))
{
  calculate();
}

void Parameters::calculate() {
  dataSize_ = calcDataSize(maxTimeStep_, samplingRate_);

  std::float64_t totalVolume = initialAutotrophicVolume_ + initialHeterotrophicVolume_;
  std::float64_t halfSaturationConstantFraction = halfSaturationConstantFraction_;

  maximumSizeClassPopulations_.resize(numberOfSizeClasses_, 0);
  remainingVolumes_.resize(numberOfSizeClasses_);
  linearFeedingDenominators_.resize(numberOfSizeClasses_);
  halfSaturationConstants_.resize(numberOfSizeClasses_);
  sizeClassMidPoints_.resize(numberOfSizeClasses_);
  sizeClassBoundaries_.resize(numberOfSizeClasses_ + 1);

  smallestVolumeExponent_ = std::log10(smallestIndividualVolume_);
  largestVolumeExponent_ = std::log10(largestIndividualVolume_);

  autotrophCellSize_ = sizeClassMidPoints_[consts::kAutotrophSizeIndex];
  preferenceDenominator_ = 2 * std::pow(preferenceFunctionWidth_, 2);

  std::float64_t sizeClassExponentIncrement = (largestVolumeExponent_ - smallestVolumeExponent_) / numberOfSizeClasses_;

  for (std::uint32_t sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
    std::float64_t sizeClassMidPointExponent = smallestVolumeExponent_ + ((sizeClassIndex + 0.5) * sizeClassExponentIncrement);
    std::float64_t sizeClassBoundaryExponent = smallestVolumeExponent_ + (sizeClassIndex * sizeClassExponentIncrement);

    sizeClassBoundaries_[sizeClassIndex] = std::pow(10, sizeClassBoundaryExponent);
    sizeClassMidPoints_[sizeClassIndex] = std::pow(10, sizeClassMidPointExponent);

    remainingVolumes_[sizeClassIndex] = totalVolume - sizeClassMidPoints_[sizeClassIndex];
    linearFeedingDenominators_[sizeClassIndex] = (2 * halfSaturationConstantFraction) * remainingVolumes_[sizeClassIndex];
    halfSaturationConstants_[sizeClassIndex] = halfSaturationConstantFraction * remainingVolumes_[sizeClassIndex];
    maximumSizeClassPopulations_[sizeClassIndex] = std::ceil(totalVolume / sizeClassMidPoints_[sizeClassIndex]);

    // Calculate inter-SizeClass preference matrices
    std::float64_t preferenceSum = 0;
    std::float64_t subjectVolumeMean = sizeClassMidPoints_[sizeClassIndex];
    interSizeClassPreferences_.resize(numberOfSizeClasses_);
    interSizeClassVolumes_.resize(numberOfSizeClasses_);
    for (std::uint32_t referenceIndex = 0; referenceIndex < numberOfSizeClasses_; ++referenceIndex) {
      std::float64_t referenceVolumeMean = sizeClassMidPoints_[referenceIndex];
      std::float64_t preferenceForReferenceSizeClass = calcPreferenceForPrey(subjectVolumeMean, referenceVolumeMean);

      preferenceSum += preferenceForReferenceSizeClass;
      interSizeClassPreferences_[sizeClassIndex].push_back(preferenceForReferenceSizeClass);
      interSizeClassVolumes_[sizeClassIndex].push_back(preferenceForReferenceSizeClass * referenceVolumeMean);
    }
  }
  std::float64_t sizeClassBoundaryExponent = smallestVolumeExponent_ + (numberOfSizeClasses_ * sizeClassExponentIncrement);
  sizeClassBoundaries_[numberOfSizeClasses_] = std::pow(10, sizeClassBoundaryExponent);

  individualHeterotrophVolume_ = autotrophCellSize_ * preferredPreyVolumeRatio_;
  individualHetertrophIndex_ = findSizeClassIndexFromVolume(individualHeterotrophVolume_);
}

std::float64_t Parameters::calcPreferenceForPrey(const std::float64_t& grazerVolume, const std::float64_t& preyVolume) const {
  return std::exp(-std::pow((std::log((preferredPreyVolumeRatio_ * preyVolume) / grazerVolume)), 2) / preferenceDenominator_);
}

std::uint32_t Parameters::findSizeClassIndexFromVolume(const std::float64_t& volume) const {
  std::uint32_t sizeClassIndex = 0;
  for (std::uint32_t index = 1; index <= numberOfSizeClasses_; ++index) {
    if (volume < getSizeClassBoundary(index)) {
      sizeClassIndex = index - 1;
      break;
    }
  }
  return sizeClassIndex;
}

const std::uint32_t& Parameters::getRandomSeed() const {
  return randomSeed_;
}

const std::uint32_t& Parameters::getMaxTimeStep() const {
  return maxTimeStep_;
}

const std::uint32_t& Parameters::getSamplingRate() const {
  return samplingRate_;
}

const std::uint32_t& Parameters::getNumberOfSizeClasses() const {
  return numberOfSizeClasses_;
}

const std::uint8_t& Parameters::getReadModelState() const {
  return readModelState_;
}

const std::uint8_t& Parameters::getWriteModelState() const {
  return writeModelState_;
}

const std::uint8_t& Parameters::getUseLinearFeeding() const {
  return useLinearFeeding_;
}

const std::float64_t& Parameters::getInitialAutotrophVolume() const {
  return initialAutotrophicVolume_;
}

const std::float64_t& Parameters::getInitialHeterotrophVolume() const {
  return initialHeterotrophicVolume_;
}

const std::float64_t& Parameters::getMinimumHeterotrophicVolume() const {
  return minimumHeterotrophicVolume_;
}

const std::float64_t& Parameters::getSmallestIndividualVolume() const {
  return smallestIndividualVolume_;
}

const std::float64_t& Parameters::getLargestIndividualVolume() const {
  return largestIndividualVolume_;
}

const std::uint32_t& Parameters::getPreferredPreyVolumeRatio() const {
  return preferredPreyVolumeRatio_;
}

const std::float64_t& Parameters::getPreferenceFunctionWidth() const {
  return preferenceFunctionWidth_;
}

const std::float64_t& Parameters::getSizeClassSubsetFraction() const {
  return sizeClassSubsetFraction_;
}

const std::float64_t& Parameters::getHalfSaturationConstantFraction() const {
  return halfSaturationConstantFraction_;
}

const std::float64_t& Parameters::getAssimilationEfficiency() const {
  return assimilationEfficiency_;
}

const std::float64_t& Parameters::getFractionalMetabolicExpense() const {
  return fractionalMetabolicExpense_;
}

const std::float64_t& Parameters::getMetabolicIndex() const {
  return metabolicIndex_;
}

const std::float64_t& Parameters::getMutationProbability() const {
  return mutationProbability_;
}

const std::float64_t& Parameters::getMutationStandardDeviation() const {
  return mutationStandardDeviation_;
}

const std::uint32_t& Parameters::getDataSize() const {
  return dataSize_;
}

const std::vector<std::vector<std::float64_t>>& Parameters::getInterSizeClassPreferences() const {
  return interSizeClassPreferences_;
}

std::vector<std::vector<std::float64_t>>& Parameters::getInterSizeClassPreferences() {
  return interSizeClassPreferences_;
}

const std::vector<std::vector<std::float64_t>>& Parameters::getInterSizeClassVolumes() const {
  return interSizeClassVolumes_;
}

std::vector<std::vector<std::float64_t>>& Parameters::getInterSizeClassVolumes() {
  return interSizeClassVolumes_;
}

const std::uint32_t& Parameters::getMaximumSizeClassPopulation(const std::uint32_t& i) const {
  return maximumSizeClassPopulations_.at(i);
}

const std::vector<std::float64_t>& Parameters::getSizeClassBoundaries() const {
  return sizeClassBoundaries_;
}

const std::vector<std::float64_t>& Parameters::getSizeClassMidPoints() const {
  return sizeClassMidPoints_;
}

const std::float64_t& Parameters::getSizeClassBoundary(const std::uint32_t& i) const {
  return sizeClassBoundaries_.at(i);
}

const std::float64_t& Parameters::getSizeClassMidPoint(const std::uint32_t& i) const {
  return sizeClassMidPoints_.at(i);
}

const std::vector<std::float64_t>& Parameters::getLinearFeedingDenominators() const {
  return linearFeedingDenominators_;
}

const std::vector<std::float64_t>& Parameters::getHalfSaturationConstants() const {
  return halfSaturationConstants_;
}

const std::float64_t& Parameters::getSmallestVolumeExponent() const {
  return smallestVolumeExponent_;
}

const std::float64_t& Parameters::getLargestVolumeExponent() const {
  return largestVolumeExponent_;
}

const std::float64_t& Parameters::getAutotrophCellSize() const {
  return autotrophCellSize_;
}

const std::float64_t& Parameters::getIndividualHeterotrophVolume() const {
  return individualHeterotrophVolume_;
}

const std::uint32_t& Parameters::getIndividualHeterotrophIndex() const {
  return individualHetertrophIndex_;
}
