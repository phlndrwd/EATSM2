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

#include "Parameters.h"

namespace {
SizeClass sizeClassGenerator(Nutrient& nutrient, Parameters& params, EcologicalData& data, EcologicalFunctions& functions,
			     RandomSimple& random, const double initialAutotrophVolume,
			     const double initialHeterotrophVolume, unsigned& index) {
  SizeClass sizeClass(nutrient, params, data, functions, initialAutotrophVolume,
                      initialHeterotrophVolume, index, random.getUniformInt(1, UINT_MAX));
  ++index;
  return sizeClass;
}
}  // anonymous namespace

Life::Life(Nutrient& nutrient, Parameters& params) :
    nutrient_(nutrient),
    params_(params),
    data_(params_),
    functions_(data_, params_),
    random_(params.getRandomSeed()),  // Is this the first time random is used?
    algorithm_(data_, functions_, params_, random_.getUniformInt(1, UINT_MAX)),
    numberOfSizeClasses_(params_.getNumberOfSizeClasses()) {
  unsigned autotrophIndex = 0;  // Hard-coded value
  double idealInitialVolume = params.getSmallestIndividualVolume() * params.getPreferredPreyVolumeRatio();
  unsigned heterotrophIndex = findSizeClassIndexFromVolume(idealInitialVolume);
  unsigned index = 0;
  std::generate_n(std::back_inserter(sizeClasses_), numberOfSizeClasses_, [&] {
    double initialAutotrophVolume = autotrophIndex != index ? 0 : params.getInitialAutotrophVolume();
    double initialHeterotrophVolume = heterotrophIndex != index ? 0 : params.getInitialHeterotrophVolume();
    return sizeClassGenerator(nutrient_, params_, data_, functions_, random_,
                              initialAutotrophVolume, initialHeterotrophVolume, index);
  });
}

void Life::update() {
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& sizeClass) {

    // This call replaces Heterotrophs.Feeding in EATSM1.
    algorithm_.update(sizeClasses_, sizeClass);  // PJU FIX - Finish feeding functions here

    std::vector<structs::MovingHeterotroph> movingHeterotrophs;
    sizeClass.update(movingHeterotrophs);  // PJU FIX - Correcrly populate movingHeterotrophs

    for (const auto& movingHeterotroph : movingHeterotrophs) {
      // PJU FIX - Determine SizeClassIndex here.

      if (movingHeterotroph.direction_ == enums::eMoveDown) {
        // Search down from current sizeClass
        auto sizeClassDownIt = sizeClasses_.rbegin();
        std::advance(sizeClassDownIt, numberOfSizeClasses_ + sizeClass.getIndex() + 1);
        std::for_each(sizeClassDownIt, sizeClasses_.rend(), [&](SizeClass& nextSizeClass){
          //
        });




      } else if (movingHeterotroph.direction_ == enums::eMoveUp) {
        Heterotroph& heterotroph = movingHeterotroph.heterotroph_;
        std::vector<SizeClass>::iterator destSizeClass =
        std::find_if (sizeClasses_.begin(), sizeClasses_.end(), [&](SizeClass& nextSizeClass) {
          if (heterotroph.getVolumeActual() >= data_.getSizeClassBoundaries()[nextSizeClass.getIndex()]) {
            return true;
          } else {
            return false;
          }
        });
        destSizeClass = std::prev(destSizeClass);
        destSizeClass->addHeterotroph(heterotroph);

        //// Search up from current sizeClass
        //auto sizeClassUpIt = sizeClasses_.begin();
        //std::advance(sizeClassUpIt, sizeClass.getIndex());
        //std::for_each(sizeClassUpIt, sizeClasses_.end(), [&](SizeClass& nextSizeClass){
        //  //
        //});
      }

      //std::advance(sizeClassIt, heterotroph.getSizeClassIndex());
      //sizeClassIt->addHeterotroph(movingHeterotroph.heterotroph_);
    }
  });
}

unsigned Life::findSizeClassIndexFromVolume(const double volume) const {
  unsigned sizeClassIndex = 0;

  for (unsigned index = 1; index <= numberOfSizeClasses_; ++index) {
    if (volume < data_.getSizeClassBoundaries()[index]) {
      sizeClassIndex = index - 1;
      break;
    }
  }
  return sizeClassIndex;
}
