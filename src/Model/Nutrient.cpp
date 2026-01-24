/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Nutrient.h"

#include <iostream>

Nutrient::Nutrient(std::uint32_t dataSize) :
      volume_(0),  // No option to set initial nutrient volume
      toFlux_(0),
      buffVolume_("nutrientVolume", "totals", dataSize, volume_) {
  std::cout << "Nutrient pool created." << std::endl;
}

const std::float64_t Nutrient::getVolume() const {
  return volume_;
}

void Nutrient::addToVolume(const std::float64_t volume) {
  volume_ += volume;
  toFlux_ += volume;
}

void Nutrient::subtractFromVolume(const std::float64_t volume) {
  volume_ -= volume;
}
