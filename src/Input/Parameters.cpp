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
#include "Enums.h"

namespace {
std::uint64_t calcDataSize(const std::uint64_t maxTimeStep, const std::uint64_t samplingRate) {
  if (maxTimeStep == 0) {
      throw std::invalid_argument("Numerator must be greater than zero...");
    }
  if (samplingRate == 0) {
      throw std::invalid_argument("Division by zero is not allowed...");
    }
  std::float64_t result = static_cast<std::float64_t>(maxTimeStep) /
                  static_cast<std::float64_t>(samplingRate);
  return static_cast<std::uint64_t>(std::ceil(result));
}
}

Parameters::Parameters(jino::Data& input):
    randomSeed_(input.getValue<std::uint8_t>(consts::kParamNames.at(enums::eRandomSeed))),
    maxTimeStep_(input.getValue<std::uint64_t>(consts::kParamNames.at(enums::eMaxTimeStep))),
    samplingRate_(input.getValue<std::uint32_t>(consts::kParamNames.at(enums::eSamplingRate))),
    numberOfSizeClasses_(input.getValue<std::uint32_t>(consts::kParamNames.at(enums::eNumberOfSizeClasses))),

    readModelState_(input.getValue<std::uint8_t>(consts::kParamNames.at(enums::eReadModelState))),
    writeModelState_(input.getValue<std::uint8_t>(consts::kParamNames.at(enums::eWriteModelState))),
    useLinearFeeding_(input.getValue<std::uint8_t>(consts::kParamNames.at(enums::eUseLinearFeeding))),

    initialAutotrophicVolume_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eInitialAutotrophicVolume))),
    initialHeterotrophicVolume_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eInitialHeterotrophicVolume))),
    minimumHeterotrophicVolume_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eMinimumHeterotrophicVolume))),

    smallestIndividualVolume_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eSmallestIndividualVolume))),
    largestIndividualVolume_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eLargestIndividualVolume))),
    sizeClassSubsetFraction_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eSizeClassSubsetFraction))),
    halfSaturationConstantFraction_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eHalfSaturationConstantFraction))),

    preferredPreyVolumeRatio_(input.getValue<std::uint32_t>(consts::kParamNames.at(enums::ePreferredPreyVolumeRatio))),
    preferenceFunctionWidth_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::ePreferenceFunctionWidth))),

    assimilationEfficiency_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eAssimilationEfficiency))),
    fractionalMetabolicExpense_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eFractionalMetabolicExpense))),
    metabolicIndex_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eMetabolicIndex))),

    mutationProbability_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eMutationProbability))),
    mutationStandardDeviation_(input.getValue<std::float64_t>(consts::kParamNames.at(enums::eMutationStandardDeviation)))
{
  calculate();
}

void Parameters::calculate() {
  dataSize_ = calcDataSize(maxTimeStep_, samplingRate_);

  std::uint32_t numberOfSizeClasses = numberOfSizeClasses_;
  std::float64_t totalVolume = initialAutotrophicVolume_ + initialHeterotrophicVolume_;
  std::float64_t halfSaturationConstantFraction = halfSaturationConstantFraction_;

  maximumSizeClassPopulations_.resize(numberOfSizeClasses, 0);
  remainingVolumes_.resize(numberOfSizeClasses);
  linearFeedingDenominators_.resize(numberOfSizeClasses);
  halfSaturationConstants_.resize(numberOfSizeClasses);
  sizeClassMidPoints_.resize(numberOfSizeClasses);
  sizeClassBoundaries_.resize(numberOfSizeClasses + 1);

  smallestVolumeExponent_ = std::log10(smallestIndividualVolume_);
  largestVolumeExponent_ = std::log10(largestIndividualVolume_);

  std::float64_t sizeClassExponentIncrement = (largestVolumeExponent_ - smallestVolumeExponent_) / numberOfSizeClasses;
  for (std::uint32_t sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses; ++sizeClassIndex) {
      std::float64_t sizeClassMidPointExponent = smallestVolumeExponent_ + ((sizeClassIndex + 0.5) * sizeClassExponentIncrement);
      std::float64_t sizeClassBoundaryExponent = smallestVolumeExponent_ + (sizeClassIndex * sizeClassExponentIncrement);

      sizeClassBoundaries_[sizeClassIndex] = std::pow(10, sizeClassBoundaryExponent);
      sizeClassMidPoints_[sizeClassIndex] = std::pow(10, sizeClassMidPointExponent);

      remainingVolumes_[sizeClassIndex] = totalVolume - sizeClassMidPoints_[sizeClassIndex];
      linearFeedingDenominators_[sizeClassIndex] = (2 * halfSaturationConstantFraction) * remainingVolumes_[sizeClassIndex];
      halfSaturationConstants_[sizeClassIndex] = halfSaturationConstantFraction * remainingVolumes_[sizeClassIndex];
      maximumSizeClassPopulations_[sizeClassIndex] = std::ceil(totalVolume / sizeClassMidPoints_[sizeClassIndex]);
    }
  std::float64_t sizeClassBoundaryExponent = smallestVolumeExponent_ + (numberOfSizeClasses * sizeClassExponentIncrement);
  sizeClassBoundaries_[numberOfSizeClasses] = std::pow(10, sizeClassBoundaryExponent);

  autotrophCellSize_ = sizeClassMidPoints_[consts::kAutotrophSizeIndex];
}

const std::uint32_t& Parameters::getRandomSeed() const {
  return randomSeed_;
}

const std::uint64_t& Parameters::getMaxTimeStep() const {
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

const std::uint64_t& Parameters::getDataSize() const {
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

const std::vector<std::uint32_t>& Parameters::getMaximumSizeClassPopulations() const {
  return maximumSizeClassPopulations_;
}

const std::uint32_t& Parameters::getMaximumSizeClassPopulation(const std::uint64_t& i) const {
  return maximumSizeClassPopulations_.at(i);
}

const std::vector<std::float64_t>& Parameters::getSizeClassBoundaries() const {
  return sizeClassBoundaries_;
}

const std::vector<std::float64_t>& Parameters::getSizeClassMidPoints() const {
  return sizeClassMidPoints_;
}

const std::float64_t& Parameters::getSizeClassBoundary(const std::uint64_t& i) const {
  return sizeClassBoundaries_.at(i);
}

const std::float64_t& Parameters::getSizeClassMidPoint(const std::uint64_t& i) const {
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
