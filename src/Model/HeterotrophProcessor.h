#ifndef HETEROTROPHPROCESSOR
#define HETEROTROPHPROCESSOR

#include <vector>

class Heterotroph;
class RandomSimple;

class HeterotrophProcessor {
 public:
  HeterotrophProcessor();

  double calculateMetabolicDeduction(const Heterotroph&) const;
  bool updateSizeClassIndex(Heterotroph&) const;
  unsigned directionIndividualShouldMoveSizeClasses(const Heterotroph&) const;
  unsigned findIndividualSizeClassIndex(const Heterotroph&, unsigned) const;




  double calculatePreferenceForPrey(const double, const double) const;
  double calculateFeedingProbability(const unsigned, const double);

  double calculateStarvationProbability(const Heterotroph*) const;

  unsigned findSizeClassIndexFromVolume(const double) const;

  void updateHerbivoreTrophicIndex(Heterotroph*);
  void updateCarnivoreTrophicIndex(Heterotroph*, const Heterotroph*);

  double calculateHerbivoreTrophicIndex(const double) const;
  double calculateCarnivoreTrophicIndex(const double, const double) const;

  double traitValueToVolume(const double) const;
  double volumeToTraitValue(const double) const;

  int roundWithProbability(RandomSimple&, const double) const;

 private:
  double calculateFeedingProbabilityLinear(const unsigned, const double);
  double calculateFeedingProbabilityNonLinear(const unsigned, const double);

  double calculateLinearStarvation(const double, const double, const double, const double) const;
  double calculateBetaExponentialStarvation(const double, const double, const double, const double) const;

  typedef double (HeterotrophProcessor::*function)(const unsigned, const double);
  double (HeterotrophProcessor::*fStarvationProbability)(const unsigned, const double);

  const std::vector<double> sizeClassBoundaries_;
  const std::vector<double> linearFeedingDenominators_;
  const std::vector<double> halfSaturationConstants_;

  const double preferredPreyVolumeRatio_;
  const double preferenceFunctionWidth_;
  const double fractionalMetabolicExpense_;
  const double metabolicIndex_;
  const double numberOfSizeClasses_;

  const double largestVolumeExponent_;
  const double smallestVolumeExponent_;

  const double preferenceDenominator_;
};

#endif
