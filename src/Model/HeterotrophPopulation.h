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

enum eGrowthTrajectory {
  eStatic,
  eGrowing,
  eShrinking
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

  explicit HeterotrophPopulation(Nutrient*, Parameters*, const std::float64_t,
                                 const std::uint32_t, const std::uint32_t);

  void update();

  void metabolisation();
  void starvation();
  void reproduction();
  void whoIsMoving(std::vector<MovingHeterotroph>&);

  std::uint32_t getIndex() const;
  std::uint32_t getPopulationSize() const;
  std::uint32_t getLivingIndex(const std::uint32_t);

  const Heterotroph& getHeterotroph(const std::uint32_t) const;
  Heterotroph& getHeterotroph(const std::uint32_t);
  Heterotroph& getRandomHeterotroph(std::uint32_t&);  // Store living index for possible removal

  SizeClass& getHeterotrophs();

  std::float64_t getVolume() const;

  void addHeterotroph(std::unique_ptr<Heterotroph>);
  void killHeterotroph(const std::uint32_t);
  void removeDead();

 private:
  void populate(Parameters* params);
  void starve(const std::uint32_t);

  Nutrient* nutrient_;
  Functions functions_;

  RandomSimple random_;
  std::vector<SizeClass> sizeClasses_;
  std::vector<std::uint32_t> deadIndices_;

  std::uint32_t numberOfSizeClasses_;

};

#endif // SIZECLASS_H
