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

class Parameters {
 public:
  explicit Parameters(jino::Data&);
  ~Parameters();

  const std::uint32_t& getRandomSeed() const;
  const std::uint64_t& getMaxTimeStep() const;
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

 private:
  std::uint32_t randomSeed_;
  std::uint64_t maxTimeStep_;
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
};

#endif
