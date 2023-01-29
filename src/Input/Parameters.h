#ifndef PARAMETERS
#define PARAMETERS

#include "Constants.h"
#include "Types.h"

class Parameters {
 public:
  ~Parameters();

  static Types::ParametersPointer Get();

  bool Initialise(const Types::StringMatrix&);

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

  const Types::DoubleVector& GetSizeClassBoundaries();
  const Types::DoubleVector& GetSizeClassMidPoints();

  const Types::DoubleVector& GetLinearFeedingDenominators();
  const Types::DoubleVector& GetHalfSaturationConstants();

  const Types::UnsignedVector& GetMaximumSizeClassPopulations();

  const Types::DoubleVector& GetInterSizeClassPreferenceVector(const unsigned) const;
  const Types::DoubleVector& GetInterSizeClassVolumeVector(const unsigned) const;

  const Types::DoubleMatrix& GetInterSizeClassPreferenceMatrix() const;
  const Types::DoubleMatrix& GetInterSizeClassVolumeMatrix() const;

 private:
  Parameters();
  void CalculateParameters();
  bool IsInitialised();

  static Types::ParametersPointer mThis;

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
  Types::UnsignedVector mMaximumSizeClassPopulations;

  Types::DoubleVector mRemainingVolumes;
  Types::DoubleVector mLinearFeedingDenominators;
  Types::DoubleVector mHalfSaturationConstants;

  Types::DoubleVector mSizeClassBoundaries;
  Types::DoubleVector mSizeClassMidPoints;

  Types::DoubleMatrix mInterSizeClassPreferenceMatrix;
  Types::DoubleMatrix mInterSizeClassVolumeMatrix;

  std::array<bool, Constants::eMutationStandardDeviation + 1> mParametersInitialised;

  double mSmallestVolumeExponent;
  double mLargestVolumeExponent;
  double mTotalVolume;

  unsigned mAutotrophSizeClassIndex;
};

#endif
