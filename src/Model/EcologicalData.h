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

  const std::vector<std::vector<std::float64_t>>& getInterSizeClassPreferences() const;
  const std::vector<std::vector<std::float64_t>>& getInterSizeClassVolumes() const;

  std::vector<std::vector<std::float64_t>>& getInterSizeClassPreferences();
  std::vector<std::vector<std::float64_t>>& getInterSizeClassVolumes();

  const std::vector<std::uint32_t>& getMaximumSizeClassPopulations() const;

  const std::vector<std::float64_t>& getSizeClassBoundaries() const;
  const std::vector<std::float64_t>& getSizeClassMidPoints() const;

  const std::vector<std::float64_t>& getLinearFeedingDenominators() const;
  const std::vector<std::float64_t>& getHalfSaturationConstants() const;

  const std::float64_t& getSmallestVolumeExponent() const;
  const std::float64_t& getLargestVolumeExponent() const;

  const std::float64_t& getAutotrophCellSize() const;

private:
  void initialise(Parameters&);

  std::vector<std::vector<std::float64_t>> interSizeClassPreferences_;
  std::vector<std::vector<std::float64_t>> interSizeClassVolumes_;

  std::vector<std::uint32_t> maximumSizeClassPopulations_;

  std::vector<std::float64_t> sizeClassBoundaries_;
  std::vector<std::float64_t> sizeClassMidPoints_;

  std::vector<std::float64_t> linearFeedingDenominators_;
  std::vector<std::float64_t> halfSaturationConstants_;

  std::vector<std::float64_t> remainingVolumes_;

  std::float64_t smallestVolumeExponent_;
  std::float64_t largestVolumeExponent_;

  std::float64_t autotrophCellSize_;
};

#endif
