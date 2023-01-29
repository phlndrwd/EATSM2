#ifndef HETEROTROPHPROCESSOR
#define HETEROTROPHPROCESSOR

#include <vector>

class Heterotroph;
class RandomSimple;

class HeterotrophProcessor {
 public:
  HeterotrophProcessor();
  ~HeterotrophProcessor();

  double CalculatePreferenceForPrey(const double, const double) const;
  double CalculateFeedingProbability(const unsigned, const double);

  double CalculateMetabolicDeduction(const Heterotroph*) const;
  double CalculateStarvationProbability(const Heterotroph*) const;

  bool UpdateSizeClassIndex(Heterotroph*) const;
  unsigned FindSizeClassIndexFromVolume(const double) const;
  unsigned FindIndividualSizeClassIndex(const Heterotroph*, unsigned) const;
  unsigned DirectionIndividualShouldMoveSizeClasses(const Heterotroph*) const;

  void UpdateHerbivoreTrophicIndex(Heterotroph*);
  void UpdateCarnivoreTrophicIndex(Heterotroph*, const Heterotroph*);

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

  const std::vector<double> mSizeClassBoundaries;
  const std::vector<double> mLinearFeedingDenominators;
  const std::vector<double> mHalfSaturationConstants;

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
