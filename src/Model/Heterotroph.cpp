#include "Heterotroph.h"

#include "HeterotrophProcessor.h"
#include "Parameters.h"
#include "RandomSimple.h"

// For model initialisation.
Heterotroph::Heterotroph(const Traits& heritableTraits, const double volumeHeritable,
                         const unsigned sizeClassIndex) :
    traits_(heritableTraits.getValues(), heritableTraits.areTraitsMutant()),
    assimilationEfficiency_(Parameters::Get()->getAssimilationEfficiency()) {
  volumeHeritable_ = volumeHeritable;
  sizeClassIndex_ = sizeClassIndex;

  volumeActual_ = volumeHeritable_;
  volumeMinimum_ = volumeHeritable_ * constants::kMinimumFractionalVolume;
  volumeReproduction_ = constants::kReproductionFactor * volumeHeritable_;

  starvationMultiplier_ = 1 / (volumeHeritable_ - volumeMinimum_);

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

// For model restart.
Heterotroph::Heterotroph(const Traits& heritableTraits, const double volumeHeritable,
                         const double volumeActual, const unsigned sizeClassIndex) :
    traits_(heritableTraits),
    assimilationEfficiency_(Parameters::Get()->getAssimilationEfficiency()) {
  volumeActual_ = volumeActual;
  sizeClassIndex_ = sizeClassIndex;

  volumeHeritable_ = volumeHeritable;
  volumeMinimum_ = volumeHeritable_ * constants::kMinimumFractionalVolume;
  volumeReproduction_ = constants::kReproductionFactor * volumeHeritable_;

  starvationMultiplier_ = 1 / (volumeHeritable_ - volumeMinimum_);

  age_ = 0;
  trophicLevel_ = 0;
  hasFed_ = false;
  isDead_ = false;
}

Heterotroph::Heterotroph(const Heterotroph& individual) :
    traits_(individual.traits_),
    assimilationEfficiency_(individual.assimilationEfficiency_) {
  volumeHeritable_ = individual.volumeHeritable_;
  volumeMinimum_ = individual.volumeMinimum_;
  volumeReproduction_ = individual.volumeReproduction_;

  volumeActual_ = individual.volumeActual_;
  trophicLevel_ = individual.trophicLevel_;

  starvationMultiplier_ = individual.starvationMultiplier_;

  sizeClassIndex_ = individual.sizeClassIndex_;
  age_ = individual.age_;

  hasFed_ = individual.hasFed_;
  isDead_ = individual.isDead_;
}

Heterotroph::Heterotroph(const Heterotroph&& individual) noexcept :
    traits_(std::move(individual.traits_)),
    assimilationEfficiency_(std::move(individual.assimilationEfficiency_)) {
  volumeHeritable_ = std::move(individual.volumeHeritable_);
  volumeMinimum_ = std::move(individual.volumeMinimum_);
  volumeReproduction_ = std::move(individual.volumeReproduction_);

  volumeActual_ = std::move(individual.volumeActual_);
  trophicLevel_ = std::move(individual.trophicLevel_);

  starvationMultiplier_ = std::move(individual.starvationMultiplier_);

  sizeClassIndex_ = std::move(individual.sizeClassIndex_);
  age_ = std::move(individual.age_);

  hasFed_ = std::move(individual.hasFed_);
  isDead_ = std::move(individual.isDead_);
}

Heterotroph::~Heterotroph() {}

Heterotroph& Heterotroph::operator=(const Heterotroph& individual) {
  if (this != &individual) {
    traits_ = individual.traits_;

    volumeHeritable_ = individual.volumeHeritable_;
    volumeMinimum_ = individual.volumeMinimum_;
    volumeReproduction_ = individual.volumeReproduction_;

    volumeActual_ = individual.volumeActual_;
    trophicLevel_ = individual.trophicLevel_;

    starvationMultiplier_ = individual.starvationMultiplier_;

    sizeClassIndex_ = individual.sizeClassIndex_;
    age_ = individual.age_;

    hasFed_ = individual.hasFed_;
    isDead_ = individual.isDead_;
  }
  return *this;
}

Heterotroph& Heterotroph::operator=(const Heterotroph&& individual) {
  if (this != &individual) {
    traits_ = std::move(individual.traits_);
    assimilationEfficiency_ = std::move(individual.assimilationEfficiency_);

    volumeHeritable_ = std::move(individual.volumeHeritable_);
    volumeMinimum_ = std::move(individual.volumeMinimum_);
    volumeReproduction_ = std::move(individual.volumeReproduction_);

    volumeActual_ = std::move(individual.volumeActual_);
    trophicLevel_ = std::move(individual.trophicLevel_);

    starvationMultiplier_ = std::move(individual.starvationMultiplier_);

    sizeClassIndex_ = std::move(individual.sizeClassIndex_);
    age_ = std::move(individual.age_);

    hasFed_ = std::move(individual.hasFed_);
    isDead_ = std::move(individual.isDead_);
  }
  return *this;
}

//bool Heterotroph::operator==(const Heterotroph& individual) {
//  return (volumeActual_ == individual.volumeActual_ && volumeHeritable_ == individual.volumeHeritable_);
//}

Heterotroph* Heterotroph::getChild(RandomSimple& random, HeterotrophProcessor& heterotrophProcessor) {
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
