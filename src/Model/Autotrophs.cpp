/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Autotrophs.h"

#include <iostream>

#include "Nutrient.h"

Autotrophs::Autotrophs(Nutrient& nutrient, const std::float64_t& initialVolume) :
    nutrient_(nutrient), volume_(initialVolume) {
  std::cout << "Autotroph pool created." << std::endl;
}

void Autotrophs::update() {
  std::float64_t growthVolume = nutrient_.getVolume();
  addToVolume(growthVolume);
  nutrient_.subtractFromVolume(growthVolume);
}

void Autotrophs::addToVolume(const std::float64_t& volume) {
  volume_ += volume;
  toFlux_ += volume;
}

void Autotrophs::subtractFromVolume(const std::float64_t& volume) {
  volume_ -= volume;
}

std::float64_t& Autotrophs::getVolume() {
  return volume_;
}

const std::float64_t& Autotrophs::getVolume() const {
  return volume_;
}
