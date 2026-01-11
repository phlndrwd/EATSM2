/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Heterotroph.h"

#include "Constants.h"
#include "EcologicalFunctions.h"
#include "Parameters.h"
#include "RandomSimple.h"

// For model initialisation.
Heterotroph::Heterotroph(std::vector<std::float64_t>& traitValues, std::vector<std::uint8_t>& areTraitsMutant,
			 const std::float64_t& mutationProbability, const std::float64_t& mutationStandardDeviation,
			 const std::float64_t& volumeHeritable, const std::float64_t& assimilationEfficiency):
	traits_(traitValues, areTraitsMutant, mutationProbability, mutationStandardDeviation),
	volumeHeritable_(volumeHeritable),
	volumeActual_(volumeHeritable_),
	volumeMinimum_(volumeHeritable_ * consts::kMinimumFractionalVolume),
	volumeReproduction_(consts::kReproductionFactor * volumeHeritable_),
	assimilationEfficiency_(assimilationEfficiency) {
  volumeReproduction_ = consts::kReproductionFactor * volumeHeritable_;
  starvationMultiplier_ = 1. / (volumeHeritable_ - volumeMinimum_);
  age_ = 0;
  trophicLevel_ = 0;
  hasFed_ = false;
  isDead_ = false;
}

// For reproduction.
Heterotroph::Heterotroph(const Traits& heritableTraits, const std::float64_t& volumeHeritable, const std::float64_t& volumeActual,
                         const std::float64_t& volumeMinimum, const std::float64_t& trophicLevel, const std::float64_t& assimilationEfficiency):
        traits_(heritableTraits),
        volumeHeritable_(volumeHeritable),
        volumeActual_(volumeActual),
        volumeMinimum_(volumeMinimum),
        assimilationEfficiency_(assimilationEfficiency),
        trophicLevel_(trophicLevel) {
  volumeReproduction_ = consts::kReproductionFactor * volumeHeritable_;
  starvationMultiplier_ = 1. / (volumeHeritable_ - volumeMinimum_);
  age_ = 0;
  hasFed_ = false;
  isDead_ = false;
}

Heterotroph::Heterotroph(const Heterotroph& heterotroph) :
    traits_(heterotroph.traits_),
    assimilationEfficiency_(heterotroph.assimilationEfficiency_) {
  volumeHeritable_ = heterotroph.volumeHeritable_;
  volumeMinimum_ = heterotroph.volumeMinimum_;
  volumeReproduction_ = heterotroph.volumeReproduction_;

  volumeActual_ = heterotroph.volumeActual_;
  trophicLevel_ = heterotroph.trophicLevel_;

  starvationMultiplier_ = heterotroph.starvationMultiplier_;

  age_ = heterotroph.age_;

  hasFed_ = heterotroph.hasFed_;
  isDead_ = heterotroph.isDead_;
}

Heterotroph::Heterotroph(const Heterotroph&& heterotroph) noexcept :
    traits_(std::move(heterotroph.traits_)),
    assimilationEfficiency_(std::move(heterotroph.assimilationEfficiency_)) {
  volumeHeritable_ = std::move(heterotroph.volumeHeritable_);
  volumeMinimum_ = std::move(heterotroph.volumeMinimum_);
  volumeReproduction_ = std::move(heterotroph.volumeReproduction_);

  volumeActual_ = std::move(heterotroph.volumeActual_);
  trophicLevel_ = std::move(heterotroph.trophicLevel_);

  starvationMultiplier_ = std::move(heterotroph.starvationMultiplier_);

  age_ = std::move(heterotroph.age_);

  hasFed_ = std::move(heterotroph.hasFed_);
  isDead_ = std::move(heterotroph.isDead_);
}

Heterotroph& Heterotroph::operator=(const Heterotroph& heterotroph) {
  if (this != &heterotroph) {
    traits_ = heterotroph.traits_;

    volumeHeritable_ = heterotroph.volumeHeritable_;
    volumeMinimum_ = heterotroph.volumeMinimum_;
    volumeReproduction_ = heterotroph.volumeReproduction_;

    volumeActual_ = heterotroph.volumeActual_;
    trophicLevel_ = heterotroph.trophicLevel_;

    starvationMultiplier_ = heterotroph.starvationMultiplier_;

    age_ = heterotroph.age_;

    hasFed_ = heterotroph.hasFed_;
    isDead_ = heterotroph.isDead_;
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
    trophicLevel_ = std::move(heterotroph.trophicLevel_);

    starvationMultiplier_ = std::move(heterotroph.starvationMultiplier_);

    age_ = std::move(heterotroph.age_);

    hasFed_ = std::move(heterotroph.hasFed_);
    isDead_ = std::move(heterotroph.isDead_);
  }
  return *this;
}

std::shared_ptr<Heterotroph> Heterotroph::getChild(RandomSimple& random, const EcologicalFunctions& functions) {
  Traits childTraits = traits_.getChildTraits(random);

  std::float64_t childVolumeHeritable = 0;
  std::float64_t childVolumeActual = 0;
  std::float64_t childVolumeMinimum = 0;

  if (childTraits.isTraitMutant(enums::eVolume) == false) {
    childVolumeActual = volumeActual_ * consts::kReproductionMultiplier;
    childVolumeHeritable = volumeHeritable_;
    childVolumeMinimum = volumeMinimum_;
  } else {
    childVolumeHeritable = functions.traitValueToVolume(childTraits.getValue(enums::eVolume));
    childVolumeMinimum = childVolumeHeritable * consts::kMinimumFractionalVolume;
    if (childVolumeHeritable < volumeActual_) {
      childVolumeActual = childVolumeHeritable;
    } else {
      childVolumeActual = volumeActual_ * consts::kReproductionMultiplier;
    }
  }
  volumeActual_ = volumeActual_ - childVolumeActual;

  return std::make_shared<Heterotroph>(std::move(childTraits), childVolumeHeritable, childVolumeActual, childVolumeMinimum, trophicLevel_, assimilationEfficiency_);
}

std::float64_t Heterotroph::consumePreyVolume(const std::float64_t preyVolume) {
  std::float64_t volumeAssimilated = preyVolume * assimilationEfficiency_;
  std::float64_t wasteVolume = preyVolume - volumeAssimilated;

  volumeActual_ += volumeAssimilated;
  hasFed_ = true;

  return wasteVolume;
}

std::float64_t Heterotroph::metabolise(const std::float64_t metabolicDeduction) {
  ++age_;
  volumeActual_ -= metabolicDeduction;
  return metabolicDeduction;
}

Traits& Heterotroph::getHeritableTraits() {
  return traits_;
}

std::float64_t Heterotroph::getTrophicLevel() const {
  return trophicLevel_;
}

std::uint32_t Heterotroph::getAge() const {
  return age_;
}

bool Heterotroph::hasFed() const {
  return hasFed_;
}

bool Heterotroph::isDead() const {
  return isDead_;
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

void Heterotroph::setTrophicLevel(const std::float64_t trophicLevel) {
  trophicLevel_ = trophicLevel;
}

void Heterotroph::setAge(const std::uint32_t age) {
  age_ = age;
}

void Heterotroph::setHasFed(const bool hasFed) {
  hasFed_ = hasFed;
}

void Heterotroph::setDead() {
  isDead_ = true;
}
