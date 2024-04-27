/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Life.h"
#include "DataRecorder.h"

#include <algorithm>
#include <climits>

#include "Constants.h"
#include "Parameters.h"

namespace {
SizeClass sizeClassGenerator(Nutrient& nutrient, Parameters& params, EcologicalData& data,
			     const double initialAutotrophVolume, const double initialHeterotrophVolume,
			     std::uint32_t& index, std::uint32_t randomSeed) {
  SizeClass sizeClass(nutrient, params, data, initialAutotrophVolume, initialHeterotrophVolume,
                      index, randomSeed);
  ++index;
  return sizeClass;
}
}  // anonymous namespace

Life::Life(Nutrient& nutrient, Parameters& params) :
        nutrient_(nutrient),
        params_(params),
        data_(params_),
        random_(params.getRandomSeed()),  // Is this the first time random is used?
        algorithm_(nutrient, data_, params_, random_.getUniformInt(1, UINT_MAX)),
        numberOfSizeClasses_(params_.getNumberOfSizeClasses()),
        sizeClassFrequencies_(numberOfSizeClasses_, 0),
        sizeClassStarved_(numberOfSizeClasses_, 0) {
  std::uint32_t autotrophIndex = consts::kAutotrophSizeIndex;
  double idealInitialVolume = params.getSmallestIndividualVolume() * params.getPreferredPreyVolumeRatio();
  std::uint32_t heterotrophIndex = findSizeClassIndexFromVolume(idealInitialVolume);

  std::uint32_t index = 0;
  std::generate_n(std::back_inserter(sizeClasses_), numberOfSizeClasses_, [&] {
    double initialAutotrophVolume = autotrophIndex != index ? 0 : params.getInitialAutotrophVolume();
    double initialHeterotrophVolume = heterotrophIndex != index ? 0 : params.getInitialHeterotrophVolume();
    return sizeClassGenerator(nutrient_, params_, data_, initialAutotrophVolume, initialHeterotrophVolume,
                              index, random_.getUniformInt(1, UINT_MAX));
  });

  // PJU FIX - This is temporary!
  std::vector<float> sizeClassBoundaries(std::begin(data_.getSizeClassMidPoints()), std::end(data_.getSizeClassMidPoints()));
  std::vector<float> sizeClassMidPoints(std::begin(data_.getSizeClassBoundaries()), std::end(data_.getSizeClassBoundaries()));

  DataRecorder::get( )->setVectorDataOn("AxisSizeClassMidPointValues", sizeClassBoundaries);
  DataRecorder::get( )->setVectorDataOn("AxisSizeClassBoundaryValues", sizeClassMidPoints);
}

void Life::update() {
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
    // This call replaces Heterotrophs.Feeding in EATSM1.
    algorithm_.update(sizeClasses_, thisSizeClass);  // Equivalent to Heterotrophs.Feeding() in EATSM1
    thisSizeClass.update(movingHeterotrophs_);
  });
  moveHeterotrophs();
}

void Life::snapshot() {
  outputData_.reset();

  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
    thisSizeClass.snapshot();

    std::uint32_t index = thisSizeClass.getIndex();
    sizeClassFrequencies_[index] = (float)thisSizeClass.getOutputData().getLivingCount();

    outputData_ += thisSizeClass.getOutputData();
  });

  DataRecorder::get()->addDataTo("SizeClassPopulation", sizeClassFrequencies_);
  DataRecorder::get()->addDataTo("HeterotrophFrequency", outputData_.getLivingCount());
}

void Life::moveHeterotrophs() {
  for (const auto& movingHeterotroph : movingHeterotrophs_) {
    Heterotroph& heterotroph = movingHeterotroph.heterotroph;
    std::uint32_t searchOffSet = 0;
    if (movingHeterotroph.direction == enums::eMoveDown) {
      searchOffSet = numberOfSizeClasses_ - movingHeterotroph.origSizeClassIndex;  // Search next down
    } else if (movingHeterotroph.direction == enums::eMoveUp) {
      searchOffSet = movingHeterotroph.origSizeClassIndex + 1;  // Search next up
    }  // No need for else?
    auto sizeClassDownIt = std::next(sizeClasses_.begin(), searchOffSet);
    std::find_if (sizeClassDownIt, sizeClasses_.end(), [&](SizeClass& nextSizeClass) {
      if (heterotroph.getVolumeActual() >= data_.getSizeClassBoundaries()[nextSizeClass.getIndex()]) {
        nextSizeClass.getHeterotrophs().addHeterotroph(heterotroph);
        return true;
      } else {
        return false;
      }
    });
  }
  movingHeterotrophs_.clear();
}

std::uint32_t Life::findSizeClassIndexFromVolume(const double& volume) const {
  std::uint32_t sizeClassIndex = 0;
  for (std::uint32_t index = 1; index <= numberOfSizeClasses_; ++index) {
    if (volume < data_.getSizeClassBoundaries()[index]) {
      sizeClassIndex = index - 1;
      break;
    }
  }
  return sizeClassIndex;
}
