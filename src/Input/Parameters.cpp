/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Parameters.h"

#include "Constants.h"
#include "Enums.h"

Parameters::Parameters(jino::Data& input) {
  randomSeed_ = input.getValue<std::uint32_t>(consts::kParamNames.at(enums::eRandomSeed));
  maxTimeStep_ = input.getValue<std::uint64_t>(consts::kParamNames.at(enums::eMaxTimeStep));
  samplingRate_ = input.getValue<std::uint32_t>(consts::kParamNames.at(enums::eSamplingRate));
  numberOfSizeClasses_ = input.getValue<std::uint32_t>(consts::kParamNames.at(enums::eNumberOfSizeClasses));

  readModelState_ = input.getValue<std::uint8_t>(consts::kParamNames.at(enums::eReadModelState));
  writeModelState_ = input.getValue<std::uint8_t>(consts::kParamNames.at(enums::eWriteModelState));
  useLinearFeeding_ = input.getValue<std::uint8_t>(consts::kParamNames.at(enums::eUseLinearFeeding));

  initialAutotrophicVolume_ = input.getValue<double>(consts::kParamNames.at(enums::eInitialAutotrophicVolume));
  initialHeterotrophicVolume_ = input.getValue<double>(consts::kParamNames.at(enums::eInitialHeterotrophicVolume));
  minimumHeterotrophicVolume_ = input.getValue<double>(consts::kParamNames.at(enums::eMinimumHeterotrophicVolume));

  smallestIndividualVolume_ = input.getValue<double>(consts::kParamNames.at(enums::eSmallestIndividualVolume));
  largestIndividualVolume_ = input.getValue<double>(consts::kParamNames.at(enums::eLargestIndividualVolume));
  sizeClassSubsetFraction_ = input.getValue<double>(consts::kParamNames.at(enums::eSizeClassSubsetFraction));
  halfSaturationConstantFraction_ = input.getValue<double>(consts::kParamNames.at(enums::eHalfSaturationConstantFraction));

  preferredPreyVolumeRatio_ = input.getValue<std::uint32_t>(consts::kParamNames.at(enums::ePreferredPreyVolumeRatio));
  preferenceFunctionWidth_ = input.getValue<double>(consts::kParamNames.at(enums::ePreferenceFunctionWidth));

  assimilationEfficiency_ = input.getValue<double>(consts::kParamNames.at(enums::eAssimilationEfficiency));
  fractionalMetabolicExpense_ = input.getValue<double>(consts::kParamNames.at(enums::eFractionalMetabolicExpense));
  metabolicIndex_ = input.getValue<double>(consts::kParamNames.at(enums::eMetabolicIndex));

  mutationProbability_ = input.getValue<double>(consts::kParamNames.at(enums::eMutationProbability));
  mutationStandardDeviation_ = input.getValue<double>(consts::kParamNames.at(enums::eMutationStandardDeviation));
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

const uint8_t& Parameters::getReadModelState() const {
  return readModelState_;
}

const uint8_t& Parameters::getWriteModelState() const {
  return writeModelState_;
}

const uint8_t& Parameters::getUseLinearFeeding() const {
  return useLinearFeeding_;
}

const double& Parameters::getInitialAutotrophVolume() const {
  return initialAutotrophicVolume_;
}

const double& Parameters::getInitialHeterotrophVolume() const {
  return initialHeterotrophicVolume_;
}

const double& Parameters::getMinimumHeterotrophicVolume() const {
  return minimumHeterotrophicVolume_;
}

const double& Parameters::getSmallestIndividualVolume() const {
  return smallestIndividualVolume_;
}

const double& Parameters::getLargestIndividualVolume() const {
  return largestIndividualVolume_;
}

const std::uint32_t& Parameters::getPreferredPreyVolumeRatio() const {
  return preferredPreyVolumeRatio_;
}

const double& Parameters::getPreferenceFunctionWidth() const {
  return preferenceFunctionWidth_;
}

const double& Parameters::getSizeClassSubsetFraction() const {
  return sizeClassSubsetFraction_;
}

const double& Parameters::getHalfSaturationConstantFraction() const {
  return halfSaturationConstantFraction_;
}

const double& Parameters::getAssimilationEfficiency() const {
  return assimilationEfficiency_;
}

const double& Parameters::getFractionalMetabolicExpense() const {
  return fractionalMetabolicExpense_;
}

const double& Parameters::getMetabolicIndex() const {
  return metabolicIndex_;
}

const double& Parameters::getMutationProbability() const {
  return mutationProbability_;
}

const double& Parameters::getMutationStandardDeviation() const {
  return mutationStandardDeviation_;
}
