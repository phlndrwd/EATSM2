/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "EcologicalData.h"

#include <cmath>

#include "Constants.h"

EcologicalData::EcologicalData(Parameters& params) {
  initialise(params);
}

void EcologicalData::initialise(Parameters& params) {
  unsigned numberOfSizeClasses = params.getNumberOfSizeClasses();
  double totalVolume = params.getInitialAutotrophVolume() + params.getInitialHeterotrophVolume();
  double halfSaturationConstantFraction = params.getHalfSaturationConstantFraction();

  maximumSizeClassPopulations_.resize(numberOfSizeClasses, 0);
  remainingVolumes_.resize(numberOfSizeClasses);
  linearFeedingDenominators_.resize(numberOfSizeClasses);
  halfSaturationConstants_.resize(numberOfSizeClasses);
  sizeClassMidPoints_.resize(numberOfSizeClasses);
  sizeClassBoundaries_.resize(numberOfSizeClasses + 1);

  smallestVolumeExponent_ = std::log10(params.getSmallestIndividualVolume());
  largestVolumeExponent_ = std::log10(params.getLargestIndividualVolume());

  double sizeClassExponentIncrement = (largestVolumeExponent_ - smallestVolumeExponent_) / numberOfSizeClasses;
  for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses; ++sizeClassIndex) {
    double sizeClassMidPointExponent = smallestVolumeExponent_ + ((sizeClassIndex + 0.5) * sizeClassExponentIncrement);
    double sizeClassBoundaryExponent = smallestVolumeExponent_ + (sizeClassIndex * sizeClassExponentIncrement);

    sizeClassBoundaries_[sizeClassIndex] = std::pow(10, sizeClassBoundaryExponent);
    sizeClassMidPoints_[sizeClassIndex] = std::pow(10, sizeClassMidPointExponent);

    remainingVolumes_[sizeClassIndex] = totalVolume - sizeClassMidPoints_[sizeClassIndex];
    linearFeedingDenominators_[sizeClassIndex] = (2 * halfSaturationConstantFraction) * remainingVolumes_[sizeClassIndex];
    halfSaturationConstants_[sizeClassIndex] = halfSaturationConstantFraction * remainingVolumes_[sizeClassIndex];
    maximumSizeClassPopulations_[sizeClassIndex] = std::ceil(totalVolume / sizeClassMidPoints_[sizeClassIndex]);
  }
  double sizeClassBoundaryExponent = smallestVolumeExponent_ + (numberOfSizeClasses * sizeClassExponentIncrement);
  sizeClassBoundaries_[numberOfSizeClasses] = std::pow(10, sizeClassBoundaryExponent);

  autotrophCellSize_ = sizeClassMidPoints_[consts::kAutotrophSizeIndex];
}

const std::vector<std::vector<double>>& EcologicalData::getInterSizeClassPreferences() const {
  return interSizeClassPreferences_;
}

std::vector<std::vector<double>>& EcologicalData::getInterSizeClassPreferences() {
  return interSizeClassPreferences_;
}

const std::vector<std::vector<double>>& EcologicalData::getInterSizeClassVolumes() const {
  return interSizeClassVolumes_;
}

std::vector<std::vector<double>>& EcologicalData::getInterSizeClassVolumes() {
  return interSizeClassVolumes_;
}

const std::vector<unsigned>& EcologicalData::getMaximumSizeClassPopulations() const {
  return maximumSizeClassPopulations_;
}

const std::vector<double>& EcologicalData::getSizeClassBoundaries() const {
  return sizeClassBoundaries_;
}

const std::vector<double>& EcologicalData::getSizeClassMidPoints() const {
  return sizeClassMidPoints_;
}

const std::vector<double>& EcologicalData::getLinearFeedingDenominators() const {
  return linearFeedingDenominators_;
}

const std::vector<double>& EcologicalData::getHalfSaturationConstants() const {
  return halfSaturationConstants_;
}

const double& EcologicalData::getSmallestVolumeExponent() const {
  return smallestVolumeExponent_;
}

const double& EcologicalData::getLargestVolumeExponent() const {
  return largestVolumeExponent_;
}

const double& EcologicalData::getAutotrophCellSize() const {
  return autotrophCellSize_;
}
