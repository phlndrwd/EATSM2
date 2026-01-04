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
        sizeClassLiving_(numberOfSizeClasses_, 0),
        sizeClassDead_(numberOfSizeClasses_, 0) {
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
  /// Feeding - subset
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
    algorithm_.update(sizeClasses_, thisSizeClass);
  });
  /// Metabolisation - full set
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
    thisSizeClass.metabolisation();
  });
  /// Starvation - subset
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
    thisSizeClass.starvation();
  });
  /// Reproduction - full set
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
    thisSizeClass.reproduction();
  });

  // Movement between size classes - integrate into reproduction?
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
   thisSizeClass.whoIsMoving(movingHeterotrophs_);
  });
  moveHeterotrophs();
}

void Life::snapshot() {
  outputData_.reset();

  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& thisSizeClass) {
    OutputData& sizeClassOutput = thisSizeClass.snapshot();

    std::uint32_t index = thisSizeClass.getIndex();
    sizeClassLiving_.at(index) = (float)sizeClassOutput.getLivingCount();
    sizeClassDead_.at(index) = (float)sizeClassOutput.getDeadCount();

    outputData_ += thisSizeClass.getOutputData();
  });
  DataRecorder::get()->addDataTo("SizeClassLiving", sizeClassLiving_);
  DataRecorder::get()->addDataTo("SizeClassDead", sizeClassDead_);
  DataRecorder::get()->addDataTo("LivingCount", outputData_.getLivingCount());
  DataRecorder::get()->addDataTo("DeadCount", outputData_.getDeadCount());
}

void Life::moveHeterotrophs() {
  for (const auto& movingHeterotroph : movingHeterotrophs_) {
    Heterotroph& heterotroph = movingHeterotroph.heterotroph;
    std::uint32_t searchOffSet = 0;
    if (movingHeterotroph.growthTrajectory == enums::eShrinking) {
      searchOffSet = movingHeterotroph.prevSizeClassIndex - 1;
      auto sizeClassDownIt = std::next(sizeClasses_.rbegin(), searchOffSet);
      std::find_if(sizeClassDownIt, sizeClasses_.rend(), [&](SizeClass& prevSizeClass) {
        if (heterotroph.getVolumeActual() >= data_.getSizeClassBoundaries().at(prevSizeClass.getIndex())) {
          prevSizeClass.getHeterotrophs().addHeterotroph(heterotroph);
          return true;
        } else {
          return false;
        }
      });
    } else if (movingHeterotroph.growthTrajectory == enums::eGrowing) {
      searchOffSet = movingHeterotroph.prevSizeClassIndex + 1;
        auto sizeClassUpIt = std::next(sizeClasses_.begin(), searchOffSet);
        std::find_if(sizeClassUpIt, sizeClasses_.end(), [&](SizeClass& nextSizeClass) {
          if (heterotroph.getVolumeActual() >= data_.getSizeClassBoundaries().at(nextSizeClass.getIndex())) {
              nextSizeClass.getHeterotrophs().addHeterotroph(heterotroph);
              return true;
            } else {
              return false;
            }
        });
    }
  }
  movingHeterotrophs_.clear();
}

std::uint32_t Life::findSizeClassIndexFromVolume(const double& volume) const {
  std::uint32_t sizeClassIndex = 0;
  for (std::uint32_t index = 1; index <= numberOfSizeClasses_; ++index) {
    if (volume < data_.getSizeClassBoundaries().at(index)) {
      sizeClassIndex = index - 1;
      break;
    }
  }
  return sizeClassIndex;
}
