/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Heterotroph.h"

#include <cassert>
#include <cmath>

#include "Constants.h"
#include "Parameters.h"
#include "RandomSimple.h"

namespace {
std::float64_t traitValueToVolume(const std::float64_t& traitValue, const std::float64_t& smallestVolumeExponent, const std::float64_t& largestVolumeExponent) {
  std::float64_t volumeExponent = traitValue * (largestVolumeExponent - smallestVolumeExponent) + smallestVolumeExponent;
  return std::pow(10, volumeExponent);
}

}

// For model initialisation.
Heterotroph::Heterotroph(const std::float64_t& traitValue,
                         const std::float64_t& mutationProbability, const std::float64_t& mutationStandardDeviation,
			 const std::float64_t& volumeHeritable, const std::float64_t& assimilationEfficiency):
        traits_(traitValue, mutationProbability, mutationStandardDeviation),
	volumeHeritable_(volumeHeritable),
	volumeActual_(volumeHeritable_),
	volumeMinimum_(volumeHeritable_ * consts::kMinimumFractionalVolume),
	volumeReproduction_(consts::kReproductionFactor * volumeHeritable_),
        assimilationEfficiency_(assimilationEfficiency),
        isAlive_(true) {
  volumeReproduction_ = consts::kReproductionFactor * volumeHeritable_;
  starvationMultiplier_ = 1. / (volumeHeritable_ - volumeMinimum_);
}

// For reproduction.
Heterotroph::Heterotroph(const Traits& heritableTraits, const std::float64_t& volumeHeritable,
                         const std::float64_t& volumeActual, const std::float64_t& volumeMinimum,
                         const std::float64_t& assimilationEfficiency):
        traits_(heritableTraits),
        volumeHeritable_(volumeHeritable),
        volumeActual_(volumeActual),
        volumeMinimum_(volumeMinimum),
        assimilationEfficiency_(assimilationEfficiency),
        isAlive_(true) {
  volumeReproduction_ = consts::kReproductionFactor * volumeHeritable_;
  starvationMultiplier_ = 1. / (volumeHeritable_ - volumeMinimum_);
}

Heterotroph::Heterotroph(const Heterotroph& heterotroph) :
        traits_(heterotroph.traits_),
        assimilationEfficiency_(heterotroph.assimilationEfficiency_),
        isAlive_(true) {
  assert(this != &heterotroph);
  volumeHeritable_ = heterotroph.volumeHeritable_;
  volumeMinimum_ = heterotroph.volumeMinimum_;
  volumeReproduction_ = heterotroph.volumeReproduction_;
  volumeActual_ = heterotroph.volumeActual_;
  starvationMultiplier_ = heterotroph.starvationMultiplier_;
}

Heterotroph::Heterotroph(const Heterotroph&& heterotroph) noexcept :
        traits_(std::move(heterotroph.traits_)),
        assimilationEfficiency_(std::move(heterotroph.assimilationEfficiency_)),
        isAlive_(true) {
  assert(this != &heterotroph);
  volumeHeritable_ = std::move(heterotroph.volumeHeritable_);
  volumeMinimum_ = std::move(heterotroph.volumeMinimum_);
  volumeReproduction_ = std::move(heterotroph.volumeReproduction_);
  volumeActual_ = std::move(heterotroph.volumeActual_);
  starvationMultiplier_ = std::move(heterotroph.starvationMultiplier_);
}

Heterotroph& Heterotroph::operator=(const Heterotroph& heterotroph) {
  if (this != &heterotroph) {
    traits_ = heterotroph.traits_;
    volumeHeritable_ = heterotroph.volumeHeritable_;
    volumeMinimum_ = heterotroph.volumeMinimum_;
    volumeReproduction_ = heterotroph.volumeReproduction_;
    volumeActual_ = heterotroph.volumeActual_;
    starvationMultiplier_ = heterotroph.starvationMultiplier_;
    isAlive_ = true;
  }
  return *this;
}

Heterotroph& Heterotroph::operator=(const Heterotroph&& heterotroph) {
  if (this != &heterotroph) {
    traits_ = std::move(heterotroph.traits_);
    assimilationEfficiency_ = std::move(heterotroph.assimilationEfficiency_);
    volumeHeritable_ = std::move(heterotroph.volumeHeritable_);
    volumeMinimum_ = std::move(heterotroph.volumeMinimum_);
    volumeReproduction_ = std::move(heterotroph.volumeReproduction_);
    volumeActual_ = std::move(heterotroph.volumeActual_);
    starvationMultiplier_ = std::move(heterotroph.starvationMultiplier_);
    isAlive_ = true;
  }
  return *this;
}

std::unique_ptr<Heterotroph> Heterotroph::getChild(RandomSimple& random, const std::float64_t& smallestVolumeExponent, const std::float64_t& largestVolumeExponent) {
  Traits childTraits = traits_.getChildTraits(random);

  std::float64_t childVolumeHeritable = 0;
  std::float64_t childVolumeActual = 0;
  std::float64_t childVolumeMinimum = 0;

  if (childTraits.getValues().volumeIsMutant == false) {
    childVolumeActual = volumeActual_ * consts::kReproductionMultiplier;
    childVolumeHeritable = volumeHeritable_;
    childVolumeMinimum = volumeMinimum_;
  } else {
    childVolumeHeritable = traitValueToVolume(childTraits.getValues().volume, smallestVolumeExponent, largestVolumeExponent);
    childVolumeMinimum = childVolumeHeritable * consts::kMinimumFractionalVolume;
    if (childVolumeHeritable < volumeActual_) {
      childVolumeActual = childVolumeHeritable;
    } else {
      childVolumeActual = volumeActual_ * consts::kReproductionMultiplier;
    }
  }
  volumeActual_ = volumeActual_ - childVolumeActual;

  return std::make_unique<Heterotroph>(std::move(childTraits), std::move(childVolumeHeritable), std::move(childVolumeActual), std::move(childVolumeMinimum), assimilationEfficiency_);
}

std::float64_t Heterotroph::consumePreyVolume(const std::float64_t preyVolume) {
  std::float64_t volumeAssimilated = preyVolume * assimilationEfficiency_;
  std::float64_t wasteVolume = preyVolume - volumeAssimilated;

  volumeActual_ += volumeAssimilated;

  return wasteVolume;
}

std::float64_t Heterotroph::metabolise(const std::float64_t metabolicDeduction) {
  volumeActual_ -= metabolicDeduction;
  return metabolicDeduction;
}

Traits& Heterotroph::getHeritableTraits() {
  return traits_;
}

std::float64_t Heterotroph::getVolumeActual() const {
  return volumeActual_;
}

std::float64_t Heterotroph::getVolumeHeritable() const {
  return volumeHeritable_;
}

std::float64_t Heterotroph::getVolumeMinimum() const {
  return volumeMinimum_;
}

std::float64_t Heterotroph::getVolumeReproduction() const {
  return volumeReproduction_;
}

std::float64_t Heterotroph::getStarvationMultiplier() const {
  return starvationMultiplier_;
}

std::uint8_t Heterotroph::isAlive() const {
  return isAlive_;
}

void Heterotroph::kill() {
  isAlive_ = false;
}
