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

  bool Initialise(const std::vector<std::vector<std::string>>&);

  // Getters
  unsigned& GetRunTimeInSeconds();
  unsigned& GetRandomSeed();
  unsigned& GetSamplingRate();
  unsigned& GetNumberOfSizeClasses();

  bool GetReadModelState();
  bool GetWriteModelState();
  bool GetUseLinearFeeding();

  double& GetInitialAutotrophVolume();
  double& GetInitialHeterotrophVolume();
  double& GetMinimumHeterotrophicVolume();

  double& GetSmallestIndividualVolume();
  double& GetLargestIndividualVolume();

  unsigned& GetPreferredPreyVolumeRatio();
  double& GetPreferenceFunctionWidth();

  double& GetSizeClassSubsetFraction();
  double& GetHalfSaturationConstantFraction();

  double& GetAssimilationEfficiency();
  double& GetFractionalMetabolicExpense();
  double& GetMetabolicIndex();

  double& GetMutationProbability();
  double& GetMutationStandardDeviation();

  // Setters
  void SetRandomSeed(const unsigned);
  void SetRunTimeInSeconds(const unsigned);
  void SetSamplingRate(const unsigned);
  void SetNumberOfSizeClasses(const unsigned);

  void SetReadModelState(const bool);
  void SetWriteModelState(const bool);
  void SetUseLinearFeeding(const bool);

  void SetInitialAutotrophicVolume(const double);
  void SetInitialHeterotrophicVolume(const double);
  void SetMinimumHeterotrophicVolume(const double);

  void SetSmallestIndividualVolume(const double);
  void SetLargestIndividualVolume(const double);

  void SetPreferredPreyVolumeRatio(const unsigned);
  void SetPreferenceFunctionWidth(const double);

  void SetSizeClassSubsetFraction(const double);
  void SetHalfSaturationConstantFraction(const double);

  void SetAssimilationEfficiency(const double);
  void SetFractionalMetabolicExpense(const double);
  void SetMetabolicIndex(const double);

  void SetMutationProbability(const double);
  void SetMutationStandardDeviation(const double);

  // Calculated variables

  double& GetSmallestVolumeExponent();
  double& GetLargestVolumeExponent();

  unsigned& GetAutotrophSizeClassIndex();

  double& GetSizeClassBoundary(const unsigned);
  double& GetSizeClassMidPoint(const unsigned);

  double& GetInterSizeClassPreference(const unsigned, const unsigned);
  double& GetInterSizeClassVolume(const unsigned, const unsigned);

  double& GetTotalVolume();

  unsigned& GetMaximumSizeClassPopulation(const unsigned);
  double& GetRemainingVolume(const unsigned);
  double& GetLinearFeedingDenominator(const unsigned);
  double& GetHalfSaturationConstant(const unsigned);

  const std::vector<double>& GetSizeClassBoundaries();
  const std::vector<double>& GetSizeClassMidPoints();

  const std::vector<double>& GetLinearFeedingDenominators();
  const std::vector<double>& GetHalfSaturationConstants();

  const std::vector<unsigned>& GetMaximumSizeClassPopulations();

  const std::vector<double>& GetInterSizeClassPreferenceVector(const unsigned) const;
  const std::vector<double>& GetInterSizeClassVolumeVector(const unsigned) const;

  const std::vector<std::vector<double>>& GetInterSizeClassPreferenceMatrix() const;
  const std::vector<std::vector<double>>& GetInterSizeClassVolumeMatrix() const;

 private:
  Parameters();
  void CalculateParameters();
  bool IsInitialised();

  static Parameters* mThis;

  // User defined constants
  unsigned mRandomSeed;
  unsigned mRunTimeInSeconds;
  unsigned mSamplingRate;
  unsigned mNumberOfSizeClasses;

  bool mReadModelState;
  bool mWriteModelState;
  bool mUseLinearFeeding;

  double mInitialAutotrophicVolume;
  double mInitialHeterotrophicVolume;
  double mMinimumHeterotrophicVolume;

  double mSmallestIndividualVolume;
  double mLargestIndividualVolume;
  double mSizeClassSubsetFraction;
  double mHalfSaturationConstantFraction;

  unsigned mPreferredPreyVolumeRatio;
  double mPreferenceFunctionWidth;

  double mAssimilationEfficiency;
  double mFractionalMetabolicExpense;
  double mMetabolicIndex;

  double mMutationProbability;
  double mMutationStandardDeviation;

  // Calculated variables
  std::vector<unsigned> mMaximumSizeClassPopulations;

  std::vector<double> mRemainingVolumes;
  std::vector<double> mLinearFeedingDenominators;
  std::vector<double> mHalfSaturationConstants;

  std::vector<double> mSizeClassBoundaries;
  std::vector<double> mSizeClassMidPoints;

  std::vector<std::vector<double>> mInterSizeClassPreferenceMatrix;
  std::vector<std::vector<double>> mInterSizeClassVolumeMatrix;

  std::array<bool, Constants::eMutationStandardDeviation + 1> mParametersInitialised;

  double mSmallestVolumeExponent;
  double mLargestVolumeExponent;
  double mTotalVolume;

  unsigned mAutotrophSizeClassIndex;
};

#endif
