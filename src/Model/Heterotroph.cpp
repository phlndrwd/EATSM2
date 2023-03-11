#include "Heterotroph.h"

#include "HeterotrophProcessor.h"
#include "Parameters.h"
#include "RandomSimple.h"

// For model initialisation.
Heterotroph::Heterotroph(const Traits& heritableTraits, const double volumeHeritable, const unsigned sizeClassIndex) :
    traits_(heritableTraits.getValues(), heritableTraits.areTraitsMutant()),
    volumeHeritable_(volumeHeritable),
    sizeClassIndex_(sizeClassIndex),
    volumeActual_(volumeHeritable_),
    volumeMinimum_(volumeHeritable_ * constants::kMinimumFractionalVolume),
    volumeReproduction_(constants::kReproductionFactor * volumeHeritable_),
    assimilationEfficiency_(Parameters::Get()->getAssimilationEfficiency()),
    starvationMultiplier_(1 / (volumeHeritable_ - volumeMinimum_)) {
  age_ = 0;
  trophicLevel_ = 0;
  hasFed_ = false;
  isDead_ = false;
}

// For reproduction.
Heterotroph::Heterotroph(const Traits& heritableTraits, const double volumeHeritable,
                         const double volumeActual, const double volumeMinimum, const double trophicLevel,
                         const unsigned sizeClassIndex) :
    traits_(heritableTraits),
    assimilationEfficiency_(Parameters::Get()->getAssimilationEfficiency()) {
  volumeHeritable_ = volumeHeritable;
  volumeActual_ = volumeActual;
  volumeMinimum_ = volumeMinimum;
  trophicLevel_ = trophicLevel;
  sizeClassIndex_ = sizeClassIndex;

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

  sizeClassIndex_ = heterotroph.sizeClassIndex_;
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

  sizeClassIndex_ = std::move(heterotroph.sizeClassIndex_);
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

    sizeClassIndex_ = heterotroph.sizeClassIndex_;
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

    sizeClassIndex_ = std::move(heterotroph.sizeClassIndex_);
    age_ = std::move(heterotroph.age_);

    hasFed_ = std::move(heterotroph.hasFed_);
    isDead_ = std::move(heterotroph.isDead_);
  }
  return *this;
}

Heterotroph* Heterotroph::getChild(RandomSimple& random, const HeterotrophProcessor& heterotrophProcessor) {
  Traits childTraits = traits_.getChildTraits(random);

  double childVolumeHeritable = 0;
  double childVolumeActual = 0;
  double childVolumeMinimum = 0;

  if (childTraits.isTraitMutant(constants::eVolume) == false) {
    childVolumeActual = volumeActual_ * constants::kReproductionMultiplier;
    childVolumeHeritable = volumeHeritable_;
    childVolumeMinimum = volumeMinimum_;
  } else {
    childVolumeHeritable = heterotrophProcessor.traitValueToVolume(childTraits.getValue(constants::eVolume));
    childVolumeMinimum = childVolumeHeritable * constants::kMinimumFractionalVolume;

    if (childVolumeHeritable < volumeActual_)
      childVolumeActual = childVolumeHeritable;
    else
      childVolumeActual = volumeActual_ * constants::kReproductionMultiplier;
  }
  volumeActual_ = volumeActual_ - childVolumeActual;

  return new Heterotroph(childTraits, childVolumeHeritable, childVolumeActual, childVolumeMinimum, trophicLevel_, sizeClassIndex_);
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

unsigned Heterotroph::getSizeClassIndex() const {
  return sizeClassIndex_;
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

void Heterotroph::setSizeClassIndex(const unsigned sizeClassIndex) {
  sizeClassIndex_ = sizeClassIndex;
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
