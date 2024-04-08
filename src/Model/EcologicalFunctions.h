/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef ECOLOGICALFUNCTIONS_H
#define ECOLOGICALFUNCTIONS_H

#include <functional>
#include <vector>

#include "EcologicalData.h"
#include "Heterotroph.h"
#include "Parameters.h"

class EcologicalFunctions {
 public:
  EcologicalFunctions(EcologicalData&, Parameters&);

  double functionalResponseLinear(const std::uint32_t&, const double&) const;
  double functionalResponseNonLinear(const std::uint32_t&, const double&) const;

  double calcMetabolicDeduction(const Heterotroph&) const;
  bool updateSizeClassIndex(Heterotroph&) const;
  std::uint32_t directionIndividualShouldMoveSizeClasses(const Heterotroph&) const;
  std::uint32_t findIndividualSizeClassIndex(const Heterotroph&, std::uint32_t&) const;
  double calcStarvationProbability(const Heterotroph&) const;

  double calcPreferenceForPrey(const double&, const double&) const;
  //  double calcFeedingProbability(const std::uint32_t, const double);

  // std::uint32_t findSizeClassIndexFromVolume(const double) const;

  void updateHerbivoreTrophicIndex(Heterotroph*);
  void updateCarnivoreTrophicIndex(Heterotroph*, const Heterotroph*);

  double calcHerbivoreTrophicIndex(const double&) const;
  double calcCarnivoreTrophicIndex(const double&, const double&) const;

  double traitValueToVolume(const double&) const;
  double volumeToTraitValue(const double&) const;

 private:
  void calcPreferenceMatrices();

  double calcLinearStarvation(const double&, const double&, const double&, const double&) const;
  double calcBetaExponentialStarvation(const double&, const double&, const double&, const double&) const;

  EcologicalData& data_;

  std::function<double(const std::uint32_t, const double)> starvationProbabilityFunc_;

  const std::vector<double> sizeClassBoundaries_;
  const std::vector<double> linearFeedingDenominators_;
  const std::vector<double> halfSaturationConstants_;

  const double largestVolumeExponent_;
  const double smallestVolumeExponent_;

  const double preferredPreyVolumeRatio_;
  const double preferenceFunctionWidth_;
  const double fractionalMetabolicExpense_;
  const double metabolicIndex_;
  const double numberOfSizeClasses_;

  const double preferenceDenominator_;
};

#endif
