/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef SIZECLASS_H
#define SIZECLASS_H

#include <stdfloat>
#include <vector>

#include "Autotrophs.h"
#include "Heterotroph.h"
#include "Functions.h"
#include "SizeClass.h"
#include "Nutrient.h"
#include "Parameters.h"
#include "RandomSimple.h"

#include "Buffer.h"

enum eGrowthTrajectory {
  eStatic,
  eGrowing,
  eShrinking
};

enum eFeedingStrategy {
  eNotEating,
  eHerbivore,
  eCarnivore
};

struct PreyVolumes {
  PreyVolumes(): total(0), autotroph(0) {}
  std::float64_t total;
  std::float64_t autotroph;
};

struct MovingHeterotroph {
  std::unique_ptr<Heterotroph> heterotroph;
  std::uint32_t prevSizeClassIndex;
  eGrowthTrajectory growthTrajectory;
  MovingHeterotroph(std::unique_ptr<Heterotroph> _heterotroph,
                    std::uint32_t _prevSizeClassIndex, eGrowthTrajectory _growthTrajectory):
      heterotroph(std::move(_heterotroph)), prevSizeClassIndex(_prevSizeClassIndex),
      growthTrajectory(_growthTrajectory) {}
};

class HeterotrophPopulation {
 public:
  HeterotrophPopulation() = delete;
  HeterotrophPopulation(const HeterotrophPopulation&) = delete;
  HeterotrophPopulation& operator=(const HeterotrophPopulation&) = delete;

  HeterotrophPopulation(HeterotrophPopulation&&) noexcept = default;
  HeterotrophPopulation& operator=(HeterotrophPopulation&&) noexcept = default;

  explicit HeterotrophPopulation(Nutrient*, Autotrophs*, Parameters*, const std::uint32_t&);

  void forEachSizeClass(auto&& func) const {
    for (const auto& sizeClass : sizeClasses_) {
      func(sizeClass);
    }
  }

  void update();

  void feeding();
  void metabolisation();
  void starvation();
  void reproduction();

 private:
  void populate(Parameters* params);
  void removeDead();

  std::float64_t calcFeedingProbability(SizeClass&, std::uint32_t&, eFeedingStrategy&);
  PreyVolumes calcEffectivePreyVolumes(SizeClass&, std::vector<std::float64_t>&);
  std::uint32_t setCoupledSizeClassIndex(const std::vector<std::float64_t>&,
                                         PreyVolumes&, eFeedingStrategy&);
  void feedFromAutotrophs(Heterotroph*);
  void feedFromHeterotrophs(Heterotroph*, SizeClass&);

  Nutrient* nutrient_;
  Autotrophs* autotrophs_;

  Functions functions_;

  RandomSimple random_;
  std::vector<SizeClass> sizeClasses_;
  std::vector<MovingHeterotroph> movingHeterotrophs_;

  // Data collection
  std::uint64_t varTotalHeterotrophFrequency_;
  std::float64_t varTotalHeterotrophVolume_;

  jino::Buffer<std::uint64_t> buffTotalHeterotrophFrequency_;
  jino::Buffer<std::float64_t> buffTotalHeterotrophVolume_;

  const std::vector<std::vector<std::float64_t>> interSizeClassPreferences_;
  const std::vector<std::vector<std::float64_t>> interSizeClassVolumes_;
  const std::float64_t smallestVolumeExponent_;
  const std::float64_t largestVolumeExponent_;
  const std::float64_t autotrophCellSize_;
  const std::uint32_t numberOfSizeClasses_;
  const std::uint32_t autotrophSizeIndex_;
};

#endif // SIZECLASS_H
