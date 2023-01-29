#ifndef HETEROTROPHPROCESSOR
#define HETEROTROPHPROCESSOR

#include "RandomSimple.h"
#include "Types.h"

class HeterotrophProcessor {
 public:
  HeterotrophProcessor();
  ~HeterotrophProcessor();

  double CalculatePreferenceForPrey(const double, const double) const;
  double CalculateFeedingProbability(const unsigned, const double);

  double CalculateMetabolicDeduction(const Types::HeterotrophPointer) const;
  double CalculateStarvationProbability(const Types::HeterotrophPointer) const;

  bool UpdateSizeClassIndex(Types::HeterotrophPointer) const;
  unsigned FindSizeClassIndexFromVolume(const double) const;
  unsigned FindIndividualSizeClassIndex(const Types::HeterotrophPointer, unsigned) const;
  unsigned DirectionIndividualShouldMoveSizeClasses(const Types::HeterotrophPointer) const;

  void UpdateHerbivoreTrophicIndex(const Types::HeterotrophPointer) const;
  void UpdateCarnivoreTrophicIndex(const Types::HeterotrophPointer, const Types::HeterotrophPointer) const;

  double CalculateHerbivoreTrophicIndex(const double) const;
  double CalculateCarnivoreTrophicIndex(const double, const double) const;

  double TraitValueToVolume(const double);
  double VolumeToTraitValue(const double) const;

  int RoundWithProbability(RandomSimple&, const double) const;

 private:
  double CalculateFeedingProbabilityLinear(const unsigned, const double);
  double CalculateFeedingProbabilityNonLinear(const unsigned, const double);

  double CalculateLinearStarvation(const double, const double, const double, const double) const;
  double CalculateBetaExponentialStarvation(const double, const double, const double, const double) const;

  typedef double (HeterotrophProcessor::*function)(const unsigned, const double);
  double (HeterotrophProcessor::*fStarvationProbability)(const unsigned, const double);

  const Types::DoubleVector mSizeClassBoundaries;
  const Types::DoubleVector mLinearFeedingDenominators;
  const Types::DoubleVector mHalfSaturationConstants;

  const double mPreferredPreyVolumeRatio;
  const double mPreferenceFunctionWidth;
  const double mFractionalMetabolicExpense;
  const double mMetabolicIndex;
  const double mNumberOfSizeClasses;

  const double mLargestVolumeExponent;
  const double mSmallestVolumeExponent;

  const double mPreferenceDenominator;
};

#endif
