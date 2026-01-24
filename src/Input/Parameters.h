/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef PARAMETERS
#define PARAMETERS

#include <cstdint>
#include <stdfloat>

#include "Data.h"

enum eParameters {
  eRandomSeed,
  eMaxTimeStep,
  eSamplingRate,
  eNumberOfSizeClasses,
  eReadModelState,
  eWriteModelState,
  eUseLinearFeeding,
  eInitialAutotrophicVolume,
  eInitialHeterotrophicVolume,
  eMinimumHeterotrophicVolume,
  eSmallestIndividualVolume,
  eLargestIndividualVolume,
  eSizeClassSubsetFraction,
  eHalfSaturationConstantFraction,
  ePreferredPreyVolumeRatio,
  ePreferenceFunctionWidth,
  eAssimilationEfficiency,
  eFractionalMetabolicExpense,
  eMetabolicIndex,
  eMutationProbability,
  eMutationStandardDeviation,
  eNumberOfParamters
};

class Parameters {
 public:
  explicit Parameters(jino::Data&);

  const std::uint32_t& getRandomSeed() const;
  const std::uint32_t& getMaxTimeStep() const;
  const std::uint32_t& getSamplingRate() const;
  const std::uint32_t& getNumberOfSizeClasses() const;

  const std::uint8_t& getReadModelState() const;
  const std::uint8_t& getWriteModelState() const;
  const std::uint8_t& getUseLinearFeeding() const;

  const std::float64_t& getInitialAutotrophVolume() const;
  const std::float64_t& getInitialHeterotrophVolume() const;
  const std::float64_t& getMinimumHeterotrophicVolume() const;

  const std::float64_t& getSmallestIndividualVolume() const;
  const std::float64_t& getLargestIndividualVolume() const;

  const std::uint32_t& getPreferredPreyVolumeRatio() const;
  const std::float64_t& getPreferenceFunctionWidth() const;

  const std::float64_t& getSizeClassSubsetFraction() const;
  const std::float64_t& getHalfSaturationConstantFraction() const;

  const std::float64_t& getAssimilationEfficiency() const;
  const std::float64_t& getFractionalMetabolicExpense() const;
  const std::float64_t& getMetabolicIndex() const;

  const std::float64_t& getMutationProbability() const;
  const std::float64_t& getMutationStandardDeviation() const;

  // Calculated parameters
  void calculate();

  const std::uint32_t& getDataSize() const;

  const std::vector<std::vector<std::float64_t>>& getInterSizeClassPreferences() const;
  const std::vector<std::vector<std::float64_t>>& getInterSizeClassVolumes() const;

  std::vector<std::vector<std::float64_t>>& getInterSizeClassPreferences();
  std::vector<std::vector<std::float64_t>>& getInterSizeClassVolumes();

  const std::vector<std::uint32_t>& getMaximumSizeClassPopulations() const;
  const std::uint32_t& getMaximumSizeClassPopulation(const std::uint32_t&) const;

  const std::vector<std::float64_t>& getSizeClassBoundaries() const;
  const std::vector<std::float64_t>& getSizeClassMidPoints() const;

  const std::float64_t& getSizeClassBoundary(const std::uint32_t&) const;
  const std::float64_t& getSizeClassMidPoint(const std::uint32_t&) const;

  const std::vector<std::float64_t>& getLinearFeedingDenominators() const;
  const std::vector<std::float64_t>& getHalfSaturationConstants() const;

  const std::float64_t& getSmallestVolumeExponent() const;
  const std::float64_t& getLargestVolumeExponent() const;

  const std::float64_t& getAutotrophCellSize() const;

 private:
  std::uint32_t randomSeed_;
  std::uint32_t maxTimeStep_;
  std::uint32_t samplingRate_;
  std::uint32_t numberOfSizeClasses_;

  std::uint8_t readModelState_;
  std::uint8_t writeModelState_;
  std::uint8_t useLinearFeeding_;

  std::float64_t initialAutotrophicVolume_;
  std::float64_t initialHeterotrophicVolume_;
  std::float64_t minimumHeterotrophicVolume_;

  std::float64_t smallestIndividualVolume_;
  std::float64_t largestIndividualVolume_;
  std::float64_t sizeClassSubsetFraction_;
  std::float64_t halfSaturationConstantFraction_;

  std::uint32_t preferredPreyVolumeRatio_;
  std::float64_t preferenceFunctionWidth_;

  std::float64_t assimilationEfficiency_;
  std::float64_t fractionalMetabolicExpense_;
  std::float64_t metabolicIndex_;

  std::float64_t mutationProbability_;
  std::float64_t mutationStandardDeviation_;

  // Calculated parameters
  std::uint32_t dataSize_;

  std::vector<std::vector<std::float64_t>> interSizeClassPreferences_;
  std::vector<std::vector<std::float64_t>> interSizeClassVolumes_;

  std::vector<std::uint32_t> maximumSizeClassPopulations_;

  std::vector<std::float64_t> sizeClassBoundaries_;
  std::vector<std::float64_t> sizeClassMidPoints_;

  std::vector<std::float64_t> linearFeedingDenominators_;
  std::vector<std::float64_t> halfSaturationConstants_;

  std::vector<std::float64_t> remainingVolumes_;

  std::float64_t smallestVolumeExponent_;
  std::float64_t largestVolumeExponent_;

  std::float64_t autotrophCellSize_;
};

#endif
