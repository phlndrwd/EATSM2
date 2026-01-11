/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef NUTRIENT_H
#define NUTRIENT_H

#include <stdfloat>

#include "Parameters.h"

#include "Buffer.h"

class Nutrient {
 public:
  Nutrient(Parameters&);

  const std::float64_t& getVolume() const;

  void addToVolume(const std::float64_t&);
  void subtractFromVolume(const std::float64_t&);

 private:
  std::float64_t volume_;
  std::float64_t toFlux_;

  jino::Buffer<std::float64_t> buffVolume_;
};

#endif
