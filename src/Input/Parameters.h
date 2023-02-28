#ifndef PARAMETERS
#define PARAMETERS

#include <array>
#include <string>
#include <vector>

#include "Constants.h"

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

  double getPopulationSubsetFraction();
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

  void setPopulationSubsetFraction(const double);
  void setHalfSaturationConstantFraction(const double);

  void setAssimilationEfficiency(const double);
  void setFractionalMetabolicExpense(const double);
  void setMetabolicIndex(const double);

  void setMutationProbability(const double);
  void setMutationStandardDeviation(const double);

  // Calculated variables

  double getSmallestVolumeExponent();
  double getLargestVolumeExponent();

  unsigned getAutotrophSizeClassIndex();

  double getSizeClassBoundary(const unsigned);
  double getSizeClassMidPoint(const unsigned);

  double getInterSizeClassPreference(const unsigned, const unsigned);
  double getInterSizeClassVolume(const unsigned, const unsigned);

  double getTotalVolume();

  unsigned getMaximumSizeClassPopulation(const unsigned);
  double getRemainingVolume(const unsigned);
  double getLinearFeedingDenominator(const unsigned);
  double getHalfSaturationConstant(const unsigned);

  const std::vector<double>& getSizeClassBoundaries();
  const std::vector<double>& getSizeClassMidPoints();

  const std::vector<double>& getLinearFeedingDenominators();
  const std::vector<double>& getHalfSaturationConstants();

  const std::vector<unsigned>& getMaximumSizeClassPopulations();

  const std::vector<double>& getInterSizeClassPreferenceVector(const unsigned) const;
  const std::vector<double>& getInterSizeClassVolumeVector(const unsigned) const;

  const std::vector<std::vector<double>>& getInterSizeClassPreferenceMatrix() const;
  const std::vector<std::vector<double>>& getInterSizeClassVolumeMatrix() const;

 private:
  Parameters();
  void calculateParameters();
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
  double populationSubsetFraction_;
  double halfSaturationConstantFraction_;

  unsigned preferredPreyVolumeRatio_;
  double preferenceFunctionWidth_;

  double assimilationEfficiency_;
  double fractionalMetabolicExpense_;
  double metabolicIndex_;

  double mutationProbability_;
  double mutationStandardDeviation_;

  // Calculated variables
  std::vector<unsigned> maximumSizeClassPopulations_;

  std::vector<double> remainingVolumes_;
  std::vector<double> linearFeedingDenominators_;
  std::vector<double> halfSaturationConstants_;

  std::vector<double> sizeClassBoundaries_;
  std::vector<double> sizeClassMidPoints_;

  std::vector<std::vector<double>> interSizeClassPreferenceMatrix_;
  std::vector<std::vector<double>> interSizeClassVolumeMatrix_;

  std::array<bool, constants::eMutationStandardDeviation + 1> parametersInitialised_;

  double smallestVolumeExponent_;
  double largestVolumeExponent_;
  double totalVolume_;

  unsigned autotrophSizeClassIndex_;
};

#endif
