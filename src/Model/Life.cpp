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
        random_(params.getRandomSeed()),  // Is this the first time random is used?
        algorithm_(&nutrient, &params, random_.getUniformInt(1, UINT_MAX)),
        numberOfSizeClasses_(params.getNumberOfSizeClasses()),
        varTotalHeterotrophFrequency_(0),
        varTotalHeterotrophVolume_(0),
        varTotalAutotrophVolume_(0),
        buffTotalHeterotrophFrequency_("heterotrophFrequency", "totals", params.getDataSize(), varTotalHeterotrophFrequency_),
        buffTotalHeterotrophVolume_("heterotrophVolume", "totals", params.getDataSize(), varTotalHeterotrophVolume_),
        buffTotalAutotrophVolume_("autotrophVolume", "totals", params.getDataSize(), varTotalAutotrophVolume_){
  std::uint32_t autotrophIndex = consts::kAutotrophSizeIndex;
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

  std::float64_t initialAutotrophVolume = autotrophIndex != index ? 0 : params.getInitialAutotrophVolume();

  // PJU FIX - This is temporary!
  std::vector<std::float64_t> sizeClassBoundaries(std::begin(params.getSizeClassMidPoints()), std::end(params.getSizeClassMidPoints()));
  std::vector<std::float64_t> sizeClassMidPoints(std::begin(params.getSizeClassBoundaries()), std::end(params.getSizeClassBoundaries()));
}

void Life::update() {
  /// Feeding - subset
  //std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
  //  algorithm_.update(sizeClasses_, thisSizeClass);
  //});
  /// Metabolisation - full set
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
    thisSizeClass.metabolisation();
  });
  ///// Starvation - subset
  //std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
  //  thisSizeClass.starvation();
  //});


  ///// Reproduction - full set.
  ///// Collect data at the same time.
  //varTotalHeterotrophFrequency_ = 0;
  //varTotalHeterotrophVolume_ = 0;
  //varTotalAutotrophVolume_ = 0;

  //std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
  //  thisSizeClass.reproduction();
  //  const std::uint64_t sizeClassHeterotrophFrequency = thisSizeClass.getHeterotrophs().getLivingCount();
  //  varTotalHeterotrophFrequency_ += sizeClassHeterotrophFrequency;
  //  varTotalHeterotrophVolume_ += sizeClassHeterotrophFrequency * params_.getSizeClassMidPoint(thisSizeClass.getIndex());
  //  //varTotalAutotrophVolume_ += thisSizeClass.getAutotrophs().getVolume();
  //  //thisSizeClass.whoIsMoving(movingHeterotrophs_);
  //});
  ////moveHeterotrophs();
}

void Life::moveHeterotrophs() {
  //for (const auto& movingHeterotroph : movingHeterotrophs_) {
  //  std::uint32_t searchOffSet = 0;
  //  if (movingHeterotroph.growthTrajectory == eShrinking) {
  //    searchOffSet = params.getNumberOfSizeClasses() - movingHeterotroph.prevSizeClassIndex;
  //    auto sizeClassDownIt = std::next(sizeClasses_.rbegin(), searchOffSet);
  //    std::find_if(sizeClassDownIt, sizeClasses_.rend(), [&](SizeClass& prevSizeClass) {
  //      if (movingHeterotroph.heterotroph->getVolumeActual() >= params.getSizeClassBoundary(prevSizeClass.getIndex())) {
  //        prevSizeClass.getHeterotrophs().addHeterotroph(std::move(movingHeterotroph.heterotroph));
  //        return true;
  //      } else {
  //        return false;
  //      }
  //    });
  //  } else if (movingHeterotroph.growthTrajectory == eGrowing) {
  //    searchOffSet = movingHeterotroph.prevSizeClassIndex + 1;
  //      auto sizeClassUpIt = std::next(sizeClasses_.begin(), searchOffSet);
  //      std::find_if(sizeClassUpIt, sizeClasses_.end(), [&](SizeClass& nextSizeClass) {
  //        if (movingHeterotroph.heterotroph->getVolumeActual() >= params.getSizeClassBoundary(nextSizeClass.getIndex())) {
  //            nextSizeClass.getHeterotrophs().addHeterotroph(std::move(movingHeterotroph.heterotroph));
  //            return true;
  //          } else {
  //            return false;
  //          }
  //      });
  //  }
  //}
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
