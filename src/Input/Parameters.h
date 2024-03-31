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
#include <string>
#include <vector>

#include "Enums.h"

class Parameters {
 public:
  ~Parameters();

  static Parameters* Get();

  bool initialise(const std::vector<std::vector<std::string>>&);

  // Getters
  unsigned getRunTimeInSeconds();
  unsigned getRandomSeed();
  unsigned getSamplingRate();
  unsigned getNumberOfSizeClasses();

  bool getReadModelState();
  bool getWriteModelState();
  bool getUseLinearFeeding();

  double getInitialAutotrophVolume();
  double getInitialHeterotrophVolume();
  double getMinimumHeterotrophicVolume();

  double getSmallestIndividualVolume();
  double getLargestIndividualVolume();

  unsigned getPreferredPreyVolumeRatio();
  double getPreferenceFunctionWidth();

  double getSizeClassSubsetFraction();
  double getHalfSaturationConstantFraction();

  double getAssimilationEfficiency();
  double getFractionalMetabolicExpense();
  double getMetabolicIndex();

  double getMutationProbability();
  double getMutationStandardDeviation();

  // Setters
  void setRandomSeed(const unsigned);
  void setRunTimeInSeconds(const unsigned);
  void setSamplingRate(const unsigned);
  void setNumberOfSizeClasses(const unsigned);

  void setReadModelState(const bool);
  void setWriteModelState(const bool);
  void setUseLinearFeeding(const bool);

  void setInitialAutotrophicVolume(const double);
  void setInitialHeterotrophicVolume(const double);
  void setMinimumHeterotrophicVolume(const double);

  void setSmallestIndividualVolume(const double);
  void setLargestIndividualVolume(const double);

  void setPreferredPreyVolumeRatio(const unsigned);
  void setPreferenceFunctionWidth(const double);

  void setSizeClassSubsetFraction(const double);
  void setHalfSaturationConstantFraction(const double);

  void setAssimilationEfficiency(const double);
  void setFractionalMetabolicExpense(const double);
  void setMetabolicIndex(const double);

  void setMutationProbability(const double);
  void setMutationStandardDeviation(const double);

 private:
  Parameters();

  bool isInitialised();

  static Parameters* this_;

  // User defined constants
  unsigned randomSeed_;
  unsigned runTimeInSeconds_;
  unsigned samplingRate_;
  unsigned numberOfSizeClasses_;

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

  unsigned preferredPreyVolumeRatio_;
  double preferenceFunctionWidth_;

  double assimilationEfficiency_;
  double fractionalMetabolicExpense_;
  double metabolicIndex_;

  double mutationProbability_;
  double mutationStandardDeviation_;

  std::array<bool, enums::eNumberOfParamters> parametersInitialised_;
};

#endif
