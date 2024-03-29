/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef TYPES
#define TYPES

namespace enums {

enum eInputParameters {
  eRandomSeed,
  eRunTimeInSeconds,
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

enum eInputParametersMetadata {
  eParameterName,
  eParameterValue
};

enum eOutputControlParameters {
  eDatumName,
  eDatumType
};

enum eFeedingStrategy {
  eHerbivore,
  eCarnivore
};

enum eTraitIndices {
  eVolume
};

// Model enums
enum eMovementDirection {
  eNoMovement,
  eMoveUp,
  eMoveDown
};

}  // namespace enums

#endif
