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

  const std::vector<std::vector<double>>& getInterSizeClassPreferences() const;
  const std::vector<std::vector<double>>& getInterSizeClassVolumes() const;

  std::vector<std::vector<double>>& getInterSizeClassPreferences();
  std::vector<std::vector<double>>& getInterSizeClassVolumes();

  const std::vector<unsigned>& getMaximumSizeClassPopulations() const;

  const std::vector<double>& getSizeClassBoundaries() const;
  const std::vector<double>& getSizeClassMidPoints() const;

  const std::vector<double>& getLinearFeedingDenominators() const;
  const std::vector<double>& getHalfSaturationConstants() const;

  const double& getSmallestVolumeExponent() const;
  const double& getLargestVolumeExponent() const;

private:
  void initialise();

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
