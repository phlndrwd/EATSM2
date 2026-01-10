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
  dataSize_ = calcDataSize(maxTimeStep_, samplingRate_);
}

Parameters::~Parameters() {}

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
