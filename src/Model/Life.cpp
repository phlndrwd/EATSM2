/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Life.h"

#include <algorithm>
#include <climits>

#include "Constants.h"
#include "Parameters.h"

Life::Life(Nutrient& nutrient, Parameters& params) :
        nutrient_(nutrient),
        params_(params),
        autotrophs_(nutrient, params.getInitialAutotrophVolume(), params.getDataSize()),
        random_(params.getRandomSeed()),  // Is this the first time random is used?
        algorithm_(&autotrophs_, &nutrient, &params, random_.getUniformInt(1, UINT_MAX)),
        numberOfSizeClasses_(params.getNumberOfSizeClasses()),
        varTotalHeterotrophFrequency_(0),
        varTotalHeterotrophVolume_(0),
        buffTotalHeterotrophFrequency_("heterotrophFrequency", "totals", params.getDataSize(), varTotalHeterotrophFrequency_),
        buffTotalHeterotrophVolume_("heterotrophVolume", "totals", params.getDataSize(), varTotalHeterotrophVolume_) {
  std::float64_t idealInitialVolume = params.getSmallestIndividualVolume() * params.getPreferredPreyVolumeRatio();
  std::uint32_t heterotrophIndex = findSizeClassIndexFromVolume(idealInitialVolume);

  std::uint32_t index = 0;
  for (index = 0; index < numberOfSizeClasses_; ++index) {
    const std::float64_t initialHeterotrophVolume = heterotrophIndex != index ? 0 : params.getInitialHeterotrophVolume();
    sizeClasses_.emplace_back(
      &nutrient_,
      &params_,
      initialHeterotrophVolume,
      index,
      random_.getUniformInt(1, UINT_MAX)
    );
  }
}

void Life::update() {
  /// Update autotrophs first, as per EATSM1
  autotrophs_.update();
  /// Feeding - subset
  algorithm_.update(sizeClasses_);
  /// Metabolisation - full set
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
    thisSizeClass.metabolisation();
  });
  /// Starvation - subset
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
    thisSizeClass.starvation();
  });
  /// Reproduction - full set.
  /// Collect data at the same time.
  varTotalHeterotrophFrequency_ = 0;
  varTotalHeterotrophVolume_ = 0;

  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
    thisSizeClass.reproduction();
    varTotalHeterotrophFrequency_ += thisSizeClass.getPopulationSize();
    varTotalHeterotrophVolume_ += thisSizeClass.getVolume();
    //thisSizeClass.whoIsMoving(movingHeterotrophs_);
  });
  //moveHeterotrophs();
}

void Life::moveHeterotrophs() {
  for (auto& moving : movingHeterotrophs_) {
    const auto volume = moving.heterotroph->getVolumeActual();
    if (moving.growthTrajectory == eGrowing) {
      for (std::uint32_t index = moving.prevSizeClassIndex + 1; index < sizeClasses_.size(); ++index) {
        if (volume >= sizeClasses_[index].getSizeClassLower()) {
          sizeClasses_[index].addHeterotroph(std::move(moving.heterotroph));
          break;
        }
      }
    } else if (moving.growthTrajectory == eShrinking) {
      for (std::int32_t index = static_cast<std::int32_t>(moving.prevSizeClassIndex) - 1; index >= 0;  --index) {
        if (volume < sizeClasses_[index].getSizeClassUpper()) {
          sizeClasses_[index].addHeterotroph(std::move(moving.heterotroph));
          break;
        }
      }
    }
  }
  movingHeterotrophs_.clear();
}

std::uint32_t Life::findSizeClassIndexFromVolume(const std::float64_t& volume) const {
  std::uint32_t sizeClassIndex = 0;
  for (std::uint32_t index = 1; index <= numberOfSizeClasses_; ++index) {
    if (volume < params_.getSizeClassBoundary(index)) {
      sizeClassIndex = index - 1;
      break;
    }
  }
  return sizeClassIndex;
}
