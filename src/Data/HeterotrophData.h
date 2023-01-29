#ifndef HETEROTROPHDATA
#define HETEROTROPHDATA

#include "Types.h"

class HeterotrophData {
 public:
  HeterotrophData();
  ~HeterotrophData();

  void RecordOutputData();

  void InitialiseDataStructures();
  void ResetDataStructures();

  double GetEffectiveSizeClassVolume(const unsigned, const unsigned) const;
  double GetEffectivePreyVolume(const unsigned);
  double GetFeedingProbability(const unsigned);
  unsigned GetCoupledSizeClassIndex(const unsigned);

  void SetEffectiveSizeClassVolume(const unsigned, const unsigned, const double);
  void SetEffectivePreyVolume(const unsigned, const double);
  void SetFeedingProbability(const unsigned, const double);
  void SetCoupledSizeClassIndex(const unsigned, const unsigned);

  void AddIndividualData(const Types::HeterotrophPointer);
  void AddSizeClassData(const unsigned, const unsigned, const double);
  void NormaliseData();

  bool AreHeterotrophsAlive() const;

  void IncrementVegetarianFrequencies(const Types::HeterotrophPointer);
  void IncrementCarnivoreFrequencies(const Types::HeterotrophPointer, const Types::HeterotrophPointer);
  void IncrementStarvedFrequencies(const unsigned);
  void IncrementParentFrequencies(const unsigned);
  void IncrementChildFrequencies(const unsigned);
  void IncrementMutantFrequency(const unsigned, const unsigned);

  void AddToTimeFeeding(const double);
  void AddToTimeMetabolising(const double);
  void AddToTimeReproducing(const double);
  void AddToTimeStarving(const double);

 private:
  void AddTrophicLevel(const double, const double, const unsigned, const unsigned);

  const Types::FloatVector mSizeClassMidPointsFloat;
  const Types::FloatVector mSizeClassBoundariesFloat;

  const double mMinimumHeterotrophicVolume;
  const double mSmallestIndividualVolume;

  const unsigned mNumberOfSizeClasses;

  Types::DoubleMatrix mEffectiveSizeClassVolumeMatrix;

  Types::FloatVector mSizeClassPopulation;
  Types::FloatVector mSizeClassHerbivoreFrequencies;
  Types::FloatVector mSizeClassCarnivoreFrequencies;
  Types::FloatVector mSizeClassPreyFrequencies;
  Types::FloatVector mSizeClassStarvedFrequencies;
  Types::FloatVector mSizeClassParentFrequencies;
  Types::FloatVector mSizeClassChildFrequencies;
  Types::FloatVector mSizeClassVolumeMutantFrequencies;
  Types::FloatVector mSizeClassVolumes;
  Types::FloatVector mSizeClassApproxVolumes;
  Types::FloatVector mSizeClassEffectivePreyVolumes;
  Types::FloatVector mSizeClassGrowthRatios;
  Types::FloatVector mSizeClassCouplings;
  Types::FloatVector mSizeClassPreyVolumeRatios;
  Types::FloatVector mSizeClassFeedingProbabilities;
  Types::FloatVector mSizeClassTrophicClassifications;
  Types::FloatVector mSizeClassAges;

  Types::FloatVector mTrophicFrequencies;
  Types::FloatVector mTrophicVolumes;
  Types::FloatVector mTrophicAges;

  unsigned mFrequency;
  double mVolume;
  double mApproxVolume;
  double mToFlux;
  double mInFlux;

  double mTimeFeeding;
  double mTimeMetabolising;
  double mTimeReproducing;
  double mTimeStarving;
};

#endif
