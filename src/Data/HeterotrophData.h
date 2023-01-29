#ifndef HETEROTROPHDATA
#define HETEROTROPHDATA

#include <vector>

class Heterotroph;

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

  void AddIndividualData(const Heterotroph*);
  void AddSizeClassData(const unsigned, const unsigned, const double);
  void NormaliseData();

  bool AreHeterotrophsAlive() const;

  void IncrementVegetarianFrequencies(const Heterotroph*);
  void IncrementCarnivoreFrequencies(const Heterotroph*, const Heterotroph*);
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

  const std::vector<float> mSizeClassMidPointsFloat;
  const std::vector<float> mSizeClassBoundariesFloat;

  const double mMinimumHeterotrophicVolume;
  const double mSmallestIndividualVolume;

  const unsigned mNumberOfSizeClasses;

  std::vector<std::vector<double>> mEffectiveSizeClassVolumeMatrix;

  std::vector<float> mSizeClassPopulation;
  std::vector<float> mSizeClassHerbivoreFrequencies;
  std::vector<float> mSizeClassCarnivoreFrequencies;
  std::vector<float> mSizeClassPreyFrequencies;
  std::vector<float> mSizeClassStarvedFrequencies;
  std::vector<float> mSizeClassParentFrequencies;
  std::vector<float> mSizeClassChildFrequencies;
  std::vector<float> mSizeClassVolumeMutantFrequencies;
  std::vector<float> mSizeClassVolumes;
  std::vector<float> mSizeClassApproxVolumes;
  std::vector<float> mSizeClassEffectivePreyVolumes;
  std::vector<float> mSizeClassGrowthRatios;
  std::vector<float> mSizeClassCouplings;
  std::vector<float> mSizeClassPreyVolumeRatios;
  std::vector<float> mSizeClassFeedingProbabilities;
  std::vector<float> mSizeClassTrophicClassifications;
  std::vector<float> mSizeClassAges;

  std::vector<float> mTrophicFrequencies;
  std::vector<float> mTrophicVolumes;
  std::vector<float> mTrophicAges;

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
