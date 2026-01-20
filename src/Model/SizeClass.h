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
#include "Heterotrophs.h"
#include "Nutrient.h"
#include "Parameters.h"
#include "RandomSimple.h"

enum eGrowthTrajectory {
  eStatic,
  eGrowing,
  eShrinking
};

struct MovingHeterotroph {
  std::shared_ptr<Heterotroph> heterotroph;
  std::uint32_t prevSizeClassIndex;
  eGrowthTrajectory growthTrajectory;
  MovingHeterotroph(std::shared_ptr<Heterotroph> _heterotroph, std::uint32_t _prevSizeClassIndex, eGrowthTrajectory _growthTrajectory):
      heterotroph(_heterotroph), prevSizeClassIndex(_prevSizeClassIndex), growthTrajectory(_growthTrajectory) {}
};

class SizeClass {
 public:
  SizeClass() = delete;
  explicit SizeClass(Nutrient&, Parameters&, const std::float64_t,
                     const std::uint32_t, const std::uint32_t);

  void populate(const std::float64_t, const std::float64_t,
                const std::float64_t, const std::float64_t);

  void metabolisation();
  void starvation();
  void reproduction();
  void whoIsMoving(std::vector<MovingHeterotroph>&);

  std::uint32_t getIndex() const;
  std::uint32_t getPopulationSize() const;
  std::uint32_t getRandomHeterotrophIndex();

  const Heterotroph& getHeterotroph(const std::uint32_t) const;
  Heterotroph& getHeterotroph(const std::uint32_t);
  Heterotrophs& getHeterotrophs();

 private:
  void starve(const std::uint32_t);

  Nutrient& nutrient_;
  Functions functions_;
  const std::uint32_t index_;  // PJU FIX - Should index_ be deprecated?

  const std::float64_t sizeClassUpper_;
  const std::float64_t sizeClassMidPoint_;
  const std::float64_t sizeClassLower_;

  const std::float64_t& smallestVolumeExponent_;
  const std::float64_t& largestVolumeExponent_;

  const std::uint32_t numberOfSizeClasses_;

  RandomSimple random_;
  Heterotrophs heterotrophs_;

};

#endif // SIZECLASS_H
