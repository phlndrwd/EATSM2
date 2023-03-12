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
