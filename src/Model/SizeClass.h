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
#include "Structs.h"

class SizeClass {
 public:
  SizeClass() = delete;
  explicit SizeClass(Nutrient&, Parameters&, const std::float64_t&,
                     const std::float64_t&, const std::uint32_t&, const std::uint32_t&);

  void populate(const std::float64_t, const std::float64_t, const std::float64_t, const std::float64_t);

  void metabolisation();
  void starvation();
  void reproduction();
  void whoIsMoving(std::vector<structs::MovingHeterotroph>&);

  std::uint32_t getIndex() const;

  Autotrophs& getAutotrophs();
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
  Autotrophs autotrophs_;
  Heterotrophs heterotrophs_;

};

#endif // SIZECLASS_H
