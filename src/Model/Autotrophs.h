/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef AUTOTROPHS_H
#define AUTOTROPHS_H

#include <cstdint>
#include <stdfloat>

#include "Buffer.h"

class Nutrient;

class Autotrophs {
 public:
  Autotrophs() = delete;
  explicit Autotrophs(Nutrient&, const std::float64_t, const std::uint32_t);

  void update();
  void addToVolume(const std::float64_t);
  void subtractFromVolume(const std::float64_t);

  std::float64_t getVolume();
  const std::float64_t getVolume() const;

 private:
  Nutrient& nutrient_;

  jino::Buffer<std::float64_t> buffVolume_;

  std::float64_t volume_;
  std::float64_t toFlux_;
};

#endif
