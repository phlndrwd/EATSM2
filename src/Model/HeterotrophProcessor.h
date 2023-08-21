#ifndef HETEROTROPHPROCESSOR
#define HETEROTROPHPROCESSOR

#include <functional>
#include <vector>

#include "Heterotroph.h"
#include "RandomSimple.h"

class HeterotrophProcessor {
 public:
  HeterotrophProcessor();

  double calculateMetabolicDeduction(const Heterotroph&) const;
  bool updateSizeClassIndex(Heterotroph&) const;
  unsigned directionIndividualShouldMoveSizeClasses(const Heterotroph&) const;
  unsigned findIndividualSizeClassIndex(const Heterotroph&, unsigned) const;
  double calculateStarvationProbability(const Heterotroph&) const;

  double calculatePreferenceForPrey(const double, const double) const;
  //  double calcFeedingProbability(const unsigned, const double);

  unsigned findSizeClassIndexFromVolume(const double) const;

  void updateHerbivoreTrophicIndex(Heterotroph*);
  void updateCarnivoreTrophicIndex(Heterotroph*, const Heterotroph*);

  double calculateHerbivoreTrophicIndex(const double) const;
  double calculateCarnivoreTrophicIndex(const double, const double) const;

  double traitValueToVolume(const double) const;
  double volumeToTraitValue(const double) const;

  int roundWithProbability(RandomSimple&, const double) const;

 private:
  //  double calcFeedingProbabilityLinear(const unsigned, const double);
  //  double calcFeedingProbabilityNonLinear(const unsigned, const double);

  double calcLinearStarvation(const double, const double, const double, const double) const;
  double calcBetaExponentialStarvation(const double, const double, const double, const double) const;

  std::function<double(const unsigned, const double)> starvationProbabilityFunc_;

  const std::vector<double> sizeClassBoundaries_;
  //  const std::vector<double> linearFeedingDenominators_;
  //  const std::vector<double> halfSaturationConstants_;

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
