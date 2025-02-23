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

#include "Data.h"

class Parameters {
 public:
  explicit Parameters(jino::Data&);
  ~Parameters();

  const std::uint32_t& getRandomSeed() const;
  const std::uint64_t& getMaxTimeStep() const;
  const std::uint32_t& getSamplingRate() const;
  const std::uint32_t& getNumberOfSizeClasses() const;

  const uint8_t& getReadModelState() const;
  const uint8_t& getWriteModelState() const;
  const uint8_t& getUseLinearFeeding() const;

  const double& getInitialAutotrophVolume() const;
  const double& getInitialHeterotrophVolume() const;
  const double& getMinimumHeterotrophicVolume() const;

  const double& getSmallestIndividualVolume() const;
  const double& getLargestIndividualVolume() const;

  const std::uint32_t& getPreferredPreyVolumeRatio() const;
  const double& getPreferenceFunctionWidth() const;

  const double& getSizeClassSubsetFraction() const;
  const double& getHalfSaturationConstantFraction() const;

  const double& getAssimilationEfficiency() const;
  const double& getFractionalMetabolicExpense() const;
  const double& getMetabolicIndex() const;

  const double& getMutationProbability() const;
  const double& getMutationStandardDeviation() const;

 private:
  std::uint32_t randomSeed_;
  std::uint64_t maxTimeStep_;
  std::uint32_t samplingRate_;
  std::uint32_t numberOfSizeClasses_;

  std::uint8_t readModelState_;
  std::uint8_t writeModelState_;
  std::uint8_t useLinearFeeding_;

  double initialAutotrophicVolume_;
  double initialHeterotrophicVolume_;
  double minimumHeterotrophicVolume_;

  double smallestIndividualVolume_;
  double largestIndividualVolume_;
  double sizeClassSubsetFraction_;
  double halfSaturationConstantFraction_;

  std::uint32_t preferredPreyVolumeRatio_;
  double preferenceFunctionWidth_;

  double assimilationEfficiency_;
  double fractionalMetabolicExpense_;
  double metabolicIndex_;

  double mutationProbability_;
  double mutationStandardDeviation_;
};

#endif
