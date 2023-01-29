#ifndef HETEROTROPHDATA
#define HETEROTROPHDATA

#include <vector>

class Heterotroph;

class HeterotrophData {
 public:
  HeterotrophData();
  ~HeterotrophData();

  void recordOutputData();

  void initialiseDataStructures();
  void resetDataStructures();

  double getEffectiveSizeClassVolume(const unsigned, const unsigned) const;
  double getEffectivePreyVolume(const unsigned);
  double getFeedingProbability(const unsigned);
  unsigned getCoupledSizeClassIndex(const unsigned);

  void setEffectiveSizeClassVolume(const unsigned, const unsigned, const double);
  void setEffectivePreyVolume(const unsigned, const double);
  void setFeedingProbability(const unsigned, const double);
  void setCoupledSizeClassIndex(const unsigned, const unsigned);

  void addIndividualData(const Heterotroph*);
  void addSizeClassData(const unsigned, const unsigned, const double);
  void normaliseData();

  bool areHeterotrophsAlive() const;

  void incrementVegetarianFrequencies(const Heterotroph*);
  void incrementCarnivoreFrequencies(const Heterotroph*, const Heterotroph*);
  void incrementStarvedFrequencies(const unsigned);
  void incrementParentFrequencies(const unsigned);
  void incrementChildFrequencies(const unsigned);
  void incrementMutantFrequency(const unsigned, const unsigned);

  void addToTimeFeeding(const double);
  void addToTimeMetabolising(const double);
  void addToTimeReproducing(const double);
  void addToTimeStarving(const double);

 private:
  void addTrophicLevel(const double, const double, const unsigned, const unsigned);

  const std::vector<float> sizeClassMidPointsFloat_;
  const std::vector<float> sizeClassBoundariesFloat_;

  const double minimumHeterotrophicVolume_;
  const double smallestIndividualVolume_;

  const unsigned numberOfSizeClasses_;

  std::vector<std::vector<double>> effectiveSizeClassVolumeMatrix_;

  std::vector<float> sizeClassPopulation_;
  std::vector<float> sizeClassHerbivoreFrequencies_;
  std::vector<float> sizeClassCarnivoreFrequencies_;
  std::vector<float> sizeClassPreyFrequencies_;
  std::vector<float> sizeClassStarvedFrequencies_;
  std::vector<float> sizeClassParentFrequencies_;
  std::vector<float> sizeClassChildFrequencies_;
  std::vector<float> sizeClassVolumeMutantFrequencies_;
  std::vector<float> sizeClassVolumes_;
  std::vector<float> sizeClassApproxVolumes_;
  std::vector<float> sizeClassEffectivePreyVolumes_;
  std::vector<float> sizeClassGrowthRatios_;
  std::vector<float> sizeClassCouplings_;
  std::vector<float> sizeClassPreyVolumeRatios_;
  std::vector<float> sizeClassFeedingProbabilities_;
  std::vector<float> sizeClassTrophicClassifications_;
  std::vector<float> sizeClassAges_;

  std::vector<float> trophicFrequencies_;
  std::vector<float> trophicVolumes_;
  std::vector<float> trophicAges_;

  unsigned frequency_;
  double volume_;
  double approxVolume_;
  double toFlux_;
  double inFlux_;

  double timeFeeding_;
  double timeMetabolising_;
  double timeReproducing_;
  double timeStarving_;
};

#endif
