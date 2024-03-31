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

EcologicalData::EcologicalData(Parameters& params): params_(params) {

  unsigned numberOfSizeClasses = params_.getNumberOfSizeClasses();
  double totalVolume = params.getInitialAutotrophVolume() + params.getInitialHeterotrophVolume();
  double halfSaturationConstantFraction = params.getHalfSaturationConstantFraction();

  maximumSizeClassPopulations_.resize(numberOfSizeClasses, 0);
  remainingVolumes_.resize(numberOfSizeClasses);
  linearFeedingDenominators_.resize(numberOfSizeClasses);
  halfSaturationConstants_.resize(numberOfSizeClasses);
  sizeClassMidPoints_.resize(numberOfSizeClasses);
  sizeClassBoundaries_.resize(numberOfSizeClasses + 1);

  //// Size class sizes ////////////////////////////////////////////////////////////////////////////////////////////////

  smallestVolumeExponent_ = std::log10(params_.getSmallestIndividualVolume());
  largestVolumeExponent_ = std::log10(params_.getLargestIndividualVolume());

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

  //autotrophSizeClassIndex_ = 0;


  //// Inter-size class preferences ////////////////////////////////////////////////////////////////////////////////////
  interSizeClassPreferences_.resize(numberOfSizeClasses);
  interSizeClassVolumes_.resize(numberOfSizeClasses);

  for (unsigned subjectIndex = 0; subjectIndex < numberOfSizeClasses; ++subjectIndex) {
    double subjectVolumeMean = sizeClassMidPoints_[subjectIndex];
    double preferenceSum = 0;

    for (unsigned referenceIndex = 0; referenceIndex < numberOfSizeClasses; ++referenceIndex) {
      double referenceVolumeMean = sizeClassMidPoints_[referenceIndex];

      // PJU FIX - Calculate predator preference for prey.
      double preferenceForReferenceSizeClass = 0;


      //heterotrophProcessor.calculatePreferenceForPrey(subjectVolumeMean, referenceVolumeMean);

      preferenceSum += preferenceForReferenceSizeClass;

      interSizeClassPreferences_[subjectIndex].push_back(preferenceForReferenceSizeClass);
      interSizeClassVolumes_[subjectIndex].push_back(preferenceForReferenceSizeClass * referenceVolumeMean);
    }
  }
}

const std::vector<std::vector<double>>& EcologicalData::getInterSizeClassPreferences() const {
  return interSizeClassPreferences_;
}

const std::vector<std::vector<double>>& EcologicalData::getInterSizeClassVolumes() const {
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
