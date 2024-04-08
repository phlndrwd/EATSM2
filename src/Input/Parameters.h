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

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "Enums.h"

class Parameters {
 public:
  Parameters();
  ~Parameters();

  bool initialise(const std::vector<std::vector<std::string>>&);

  const std::uint32_t& getRunTimeInSeconds() const;
  const std::uint32_t& getRandomSeed() const;
  const std::uint32_t& getSamplingRate() const;
  const std::uint32_t& getNumberOfSizeClasses() const;

  const bool& getReadModelState() const;
  const bool& getWriteModelState() const;
  const bool& getUseLinearFeeding() const;

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

  void setRandomSeed(const std::uint32_t&);
  void setRunTimeInSeconds(const std::uint32_t&);
  void setSamplingRate(const std::uint32_t&);
  void setNumberOfSizeClasses(const std::uint32_t&);

  void setReadModelState(const bool&);
  void setWriteModelState(const bool&);
  void setUseLinearFeeding(const bool&);

  void setInitialAutotrophicVolume(const double&);
  void setInitialHeterotrophicVolume(const double&);
  void setMinimumHeterotrophicVolume(const double&);

  void setSmallestIndividualVolume(const double&);
  void setLargestIndividualVolume(const double&);

  void setPreferredPreyVolumeRatio(const std::uint32_t&);
  void setPreferenceFunctionWidth(const double&);

  void setSizeClassSubsetFraction(const double&);
  void setHalfSaturationConstantFraction(const double&);

  void setAssimilationEfficiency(const double&);
  void setFractionalMetabolicExpense(const double&);
  void setMetabolicIndex(const double&);

  void setMutationProbability(const double&);
  void setMutationStandardDeviation(const double&);

 private:
  bool isInitialised();

  std::uint32_t randomSeed_;
  std::uint32_t runTimeInSeconds_;
  std::uint32_t samplingRate_;
  std::uint32_t numberOfSizeClasses_;

  bool readModelState_;
  bool writeModelState_;
  bool useLinearFeeding_;

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

  std::array<bool, enums::eNumberOfParamters> parametersInitialised_;
};

#endif
