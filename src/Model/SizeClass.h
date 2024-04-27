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

#include <vector>

#include "Autotrophs.h"
#include "Heterotroph.h"
#include "EcologicalFunctions.h"
#include "Heterotrophs.h"
#include "Nutrient.h"
#include "OutputData.h"
#include "Parameters.h"
#include "RandomSimple.h"
#include "Structs.h"

class SizeClass {
 public:
  SizeClass() = delete;
  explicit SizeClass(Nutrient&, Parameters&, EcologicalData&, const double&,
                     const double&, const std::uint32_t&, const std::uint32_t&);

  void populate(const double, const double, const double, const double);

  void update(std::vector<structs::MovingHeterotroph>&);

  OutputData& snapshot();

  std::uint32_t getIndex() const;

  Autotrophs& getAutotrophs();
  Heterotrophs& getHeterotrophs();
  OutputData& getOutputData();

 private:
  void metabolisation();
  void starvation();
  void reproduction();
  void moveSizeClass(std::vector<structs::MovingHeterotroph>&);

  void starve(const std::uint32_t);

  Nutrient& nutrient_;
  EcologicalFunctions functions_;
  const std::uint32_t index_;  // PJU FIX - Should index_ be deprecated?

  const double sizeClassUpper_;
  const double sizeClassMidPoint_;
  const double sizeClassLower_;

  const std::uint32_t numberOfSizeClasses_;

  RandomSimple random_;
  Autotrophs autotrophs_;
  Heterotrophs heterotrophs_;
  OutputData outputData_;

};

#endif // SIZECLASS_H
