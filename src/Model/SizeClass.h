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

#include <functional>
#include <queue>
#include <vector>

#include "Autotrophs.h"
#include "Heterotroph.h"
#include "EcologicalFunctions.h"
#include "Nutrient.h"
#include "Parameters.h"
#include "RandomSimple.h"
#include "Structs.h"

class SizeClass {
 public:
  SizeClass() = delete;
  explicit SizeClass(Nutrient&, Parameters&, EcologicalData&, EcologicalFunctions&,
                     const double&, const double&, const std::uint32_t&, const std::uint32_t&);

  void populate(const double, const double, const double, const double);
  void update(std::vector<structs::MovingHeterotroph>&);

  void sizeClassSubset(std::function<void(std::uint32_t)>);

  std::size_t getPopulationSize();
  std::uint32_t getRandomHeterotrophIndex();
  Heterotroph& getRandomHeterotroph();
  Heterotroph& getRandomHeterotroph(std::uint32_t&);
  Heterotroph& getHeterotroph(const std::uint32_t);
  const Heterotroph& getHeterotroph(const std::uint32_t) const;
  void removeHeterotroph(const std::uint32_t);
  Autotrophs& getAutotrophs();

  void addHeterotroph(Heterotroph);
  std::uint32_t getIndex() const;

 private:
  void metabolisation();
  void starvation();
  void reproduction();
  void moveSizeClass(std::vector<structs::MovingHeterotroph>&);

  void starve(const std::uint32_t);

  Nutrient& nutrient_;
  EcologicalFunctions& functions_;
  const std::uint32_t index_;  // PJU FIX - Should index_ be deprecated?

  const double sizeClassUpper_;
  const double sizeClassMidPoint_;
  const double sizeClassLower_;
  const double sizeClassSubsetFraction_;

  const std::uint32_t numberOfSizeClasses_;
  const std::uint32_t maxPopulation_;

  Autotrophs autotrophs_;
  RandomSimple random_;

  std::vector<Heterotroph> heterotrophs_;
  std::vector<Heterotroph> children_;
  std::vector<std::uint32_t> alive_;
  std::queue<std::uint32_t> dead_;

};

#endif // SIZECLASS_H
