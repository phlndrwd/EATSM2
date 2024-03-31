/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef ECOLOGICALDATA_H
#define ECOLOGICALDATA_H

#include "Parameters.h"

#include <vector>

class EcologicalData {
public:
  EcologicalData(Parameters&);

  std::vector<std::vector<double>> getInterSizeClassPreferences() const;
  std::vector<std::vector<double>> getInterSizeClassVolumes() const;

  std::vector<unsigned> getMaximumSizeClassPopulations() const;

  std::vector<double> getSizeClassBoundaries() const;
  std::vector<double> getSizeClassMidPoints() const;

  std::vector<double> getLinearFeedingDenominators() const;
  std::vector<double> getHalfSaturationConstants() const;

  double getSmallestVolumeExponent() const;
  double getLargestVolumeExponent() const;

private:
  Parameters& params_;

  std::vector<std::vector<double>> interSizeClassPreferences_;
  std::vector<std::vector<double>> interSizeClassVolumes_;

  std::vector<unsigned> maximumSizeClassPopulations_;

  std::vector<double> sizeClassBoundaries_;
  std::vector<double> sizeClassMidPoints_;

  std::vector<double> linearFeedingDenominators_;
  std::vector<double> halfSaturationConstants_;

  std::vector<double> remainingVolumes_;

  double smallestVolumeExponent_;
  double largestVolumeExponent_;
};

#endif
