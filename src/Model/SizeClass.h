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
#include "RandomSimple.h"
#include "Structs.h"

class SizeClass {
 public:
  SizeClass() = delete;
  explicit SizeClass(Nutrient&, EcologicalData&, EcologicalFunctions&,
                     const double, const double, const unsigned, const unsigned);

  void populate(const double);
  void update(std::vector<structs::MovingHeterotroph>&);

  void sizeClassSubset(std::function<void(unsigned)>);

  std::size_t getPopulationSize();
  unsigned getRandomHeterotrophIndex();
  Heterotroph& getRandomHeterotroph();
  Heterotroph& getHeterotroph(const unsigned);
  const Heterotroph& getHeterotroph(const unsigned) const;
  void removeHeterotroph(const unsigned);
  Autotrophs& getAutotrophs();

  void addHeterotroph(Heterotroph);
  unsigned getIndex() const;

 private:
  void metabolisation();
  void starvation();
  void reproduction();
  void move(std::vector<structs::MovingHeterotroph>&);

  void starve(const unsigned);

  Nutrient& nutrient_;
  EcologicalFunctions& functions_;
  const unsigned index_;  // PJU FIX - Should index_ be deprecated?

  const double sizeClassUpper_;
  const double sizeClassMidPoint_;
  const double sizeClassLower_;
  const double sizeClassSubsetFraction_;

  const unsigned numberOfSizeClasses_;
  const unsigned maxPopulation_;

  Autotrophs autotrophs_;
  RandomSimple random_;

  std::vector<Heterotroph> heterotrophs_;
  std::vector<unsigned> alive_;
  std::queue<unsigned> dead_;

};

#endif // SIZECLASS_H
