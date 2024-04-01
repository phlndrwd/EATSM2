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
Heterotroph::Heterotroph(std::vector<double>& traitValues, std::vector<unsigned char>& areTraitsMutant,
			 const double& mutationProbability, const double& mutationStandardDeviation,
			 const double& volumeHeritable, const double& assimilationEfficiency):
	traits_(traitValues, areTraitsMutant, mutationProbability, mutationStandardDeviation),
	volumeHeritable_(volumeHeritable),
	volumeActual_(volumeHeritable_),
	volumeMinimum_(volumeHeritable_ * constants::kMinimumFractionalVolume),
	volumeReproduction_(constants::kReproductionFactor * volumeHeritable_),
	assimilationEfficiency_(assimilationEfficiency),
	starvationMultiplier_(1 / (volumeHeritable_ - volumeMinimum_)) {
  age_ = 0;
  trophicLevel_ = 0;
  hasFed_ = false;
  isDead_ = false;
}

// For reproduction.
Heterotroph::Heterotroph(const Traits& heritableTraits, const double& volumeHeritable, const double& volumeActual,
                         const double& volumeMinimum, const double& trophicLevel, const double& assimilationEfficiency):
        traits_(heritableTraits),
        volumeHeritable_(volumeHeritable),
        volumeActual_(volumeActual),
        volumeMinimum_(volumeMinimum),
        assimilationEfficiency_(assimilationEfficiency),
        trophicLevel_(trophicLevel) {
  volumeReproduction_ = constants::kReproductionFactor * volumeHeritable_;
  starvationMultiplier_ = 1 / (volumeHeritable_ - volumeMinimum_);
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

Heterotroph Heterotroph::getChild(RandomSimple& random, const EcologicalFunctions& functions) {
  Traits childTraits = traits_.getChildTraits(random);

  double childVolumeHeritable = 0;
  double childVolumeActual = 0;
  double childVolumeMinimum = 0;

  if (childTraits.isTraitMutant(enums::eVolume) == false) {
    childVolumeActual = volumeActual_ * constants::kReproductionMultiplier;
    childVolumeHeritable = volumeHeritable_;
    childVolumeMinimum = volumeMinimum_;
  } else {
    childVolumeHeritable = functions.traitValueToVolume(childTraits.getValue(enums::eVolume));
    childVolumeMinimum = childVolumeHeritable * constants::kMinimumFractionalVolume;
    if (childVolumeHeritable < volumeActual_) {
      childVolumeActual = childVolumeHeritable;
    } else {
      childVolumeActual = volumeActual_ * constants::kReproductionMultiplier;
    }
  }
  volumeActual_ = volumeActual_ - childVolumeActual;

  return Heterotroph(std::move(childTraits), childVolumeHeritable, childVolumeActual, childVolumeMinimum, trophicLevel_, assimilationEfficiency_);
}

double Heterotroph::consumePreyVolume(const double preyVolume) {
  double volumeAssimilated = preyVolume * assimilationEfficiency_;
  double wasteVolume = preyVolume - volumeAssimilated;

  volumeActual_ += volumeAssimilated;
  hasFed_ = true;

  return wasteVolume;
}

double Heterotroph::metabolise(const double metabolicDeduction) {
  ++age_;
  volumeActual_ -= metabolicDeduction;
  return metabolicDeduction;
}

Traits& Heterotroph::getHeritableTraits() {
  return traits_;
}

double Heterotroph::getTrophicLevel() const {
  return trophicLevel_;
}

unsigned Heterotroph::getAge() const {
  return age_;
}

bool Heterotroph::hasFed() const {
  return hasFed_;
}

bool Heterotroph::isDead() const {
  return isDead_;
}

double Heterotroph::getVolumeActual() const {
  return volumeActual_;
}

double Heterotroph::getVolumeHeritable() const {
  return volumeHeritable_;
}

double Heterotroph::getVolumeMinimum() const {
  return volumeMinimum_;
}

double Heterotroph::getVolumeReproduction() const {
  return volumeReproduction_;
}

double Heterotroph::getStarvationMultiplier() const {
  return starvationMultiplier_;
}

void Heterotroph::setTrophicLevel(const double trophicLevel) {
  trophicLevel_ = trophicLevel;
}

void Heterotroph::setAge(const unsigned age) {
  age_ = age;
}

void Heterotroph::setHasFed(const bool hasFed) {
  hasFed_ = hasFed;
}

void Heterotroph::setDead() {
  isDead_ = true;
}
