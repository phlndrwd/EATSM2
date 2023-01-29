#include "Heterotroph.h"

#include "HeritableTraits.h"
#include "HeterotrophProcessor.h"
#include "Parameters.h"

// For model initialisation.
Heterotroph::Heterotroph(const HeritableTraits& heritableTraits, const double volumeHeritable,
                         const unsigned sizeClassIndex)
    : mHeritableTraits(heritableTraits.GetValues(), heritableTraits.AreTraitsMutant()),
      mAssimilationEfficiency(Parameters::Get()->GetAssimilationEfficiency()) {
  mVolumeHeritable = volumeHeritable;
  mSizeClassIndex = sizeClassIndex;

  mVolumeActual = mVolumeHeritable;
  mVolumeMinimum = mVolumeHeritable * Constants::cMinimumFractionalVolume;
  mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

  mStarvationMultiplier = 1 / (mVolumeHeritable - mVolumeMinimum);

  mAge = 0;
  mTrophicLevel = 0;
  mHasFed = false;
  mIsDead = false;
}

// For reproduction.
Heterotroph::Heterotroph(const HeritableTraits& heritableTraits, const double volumeHeritable,
                         const double volumeActual, const double volumeMinimum, const double trophicLevel,
                         const unsigned sizeClassIndex)
    : mHeritableTraits(heritableTraits.GetValues(), heritableTraits.AreTraitsMutant()),
      mAssimilationEfficiency(Parameters::Get()->GetAssimilationEfficiency()) {
  mVolumeHeritable = volumeHeritable;
  mVolumeActual = volumeActual;
  mVolumeMinimum = volumeMinimum;
  mTrophicLevel = trophicLevel;
  mSizeClassIndex = sizeClassIndex;

  mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

  mStarvationMultiplier = 1 / (mVolumeHeritable - mVolumeMinimum);

  mAge = 0;
  mHasFed = false;
  mIsDead = false;
}

// For model restart.
Heterotroph::Heterotroph(const HeritableTraits& heritableTraits, const double volumeHeritable,
                         const double volumeActual, const unsigned sizeClassIndex)
    : mHeritableTraits(heritableTraits.GetValues(), heritableTraits.AreTraitsMutant()),
      mAssimilationEfficiency(Parameters::Get()->GetAssimilationEfficiency()) {
  mVolumeActual = volumeActual;
  mSizeClassIndex = sizeClassIndex;

  mVolumeHeritable = volumeHeritable;
  mVolumeMinimum = mVolumeHeritable * Constants::cMinimumFractionalVolume;
  mVolumeReproduction = Constants::cReproductionFactor * mVolumeHeritable;

  mStarvationMultiplier = 1 / (mVolumeHeritable - mVolumeMinimum);

  mAge = 0;
  mTrophicLevel = 0;
  mHasFed = false;
  mIsDead = false;
}

Heterotroph::Heterotroph(const HeritableTraits& heritableTraits, const double volumeHeritable,
                         const double volumeMinimum, const double volumeReproduction, const double volumeActual,
                         const double trophicLevel, const double starvationMultiplier, const unsigned sizeClassIndex,
                         const unsigned age, const bool hasFed, const bool isDead)
    : mHeritableTraits(heritableTraits.GetValues(), heritableTraits.AreTraitsMutant()),
      mAssimilationEfficiency(Parameters::Get()->GetAssimilationEfficiency()),
      mVolumeHeritable(std::move(volumeHeritable)),
      mVolumeMinimum(std::move(volumeMinimum)),
      mVolumeReproduction(std::move(volumeReproduction)),
      mVolumeActual(std::move(volumeActual)),
      mTrophicLevel(std::move(trophicLevel)),
      mStarvationMultiplier(std::move(starvationMultiplier)),
      mSizeClassIndex(std::move(sizeClassIndex)),
      mAge(std::move(age)),
      mHasFed(std::move(hasFed)),
      mIsDead(std::move(isDead)) {}

// Copy constructor
Heterotroph::Heterotroph(const Heterotroph& individual)
    : mHeritableTraits(individual.mHeritableTraits.GetValues(), individual.mHeritableTraits.AreTraitsMutant()),
      mAssimilationEfficiency(Parameters::Get()->GetAssimilationEfficiency()) {
  mVolumeHeritable = individual.mVolumeHeritable;
  mVolumeMinimum = individual.mVolumeMinimum;
  mVolumeReproduction = individual.mVolumeReproduction;

  mVolumeActual = individual.mVolumeActual;
  mTrophicLevel = individual.mTrophicLevel;

  mStarvationMultiplier = individual.mStarvationMultiplier;

  mSizeClassIndex = individual.mSizeClassIndex;
  mAge = individual.mAge;

  mHasFed = individual.mHasFed;
  mIsDead = individual.mIsDead;
}

// Move constructor
Heterotroph::Heterotroph(const Heterotroph&& individual) noexcept
    : mHeritableTraits(individual.mHeritableTraits.GetValues(), individual.mHeritableTraits.AreTraitsMutant()),
      mAssimilationEfficiency(Parameters::Get()->GetAssimilationEfficiency()) {
  mVolumeHeritable = std::move(individual.mVolumeHeritable);
  mVolumeMinimum = std::move(individual.mVolumeMinimum);
  mVolumeReproduction = std::move(individual.mVolumeReproduction);

  mVolumeActual = std::move(individual.mVolumeActual);
  mTrophicLevel = std::move(individual.mTrophicLevel);

  mStarvationMultiplier = std::move(individual.mStarvationMultiplier);

  mSizeClassIndex = std::move(individual.mSizeClassIndex);
  mAge = std::move(individual.mAge);

  mHasFed = std::move(individual.mHasFed);
  mIsDead = std::move(individual.mIsDead);
}

Heterotroph::~Heterotroph() {}

Types::HeterotrophPointer Heterotroph::Reproduce(RandomSimple& random, HeterotrophProcessor& heterotrophProcessor) {
  HeritableTraits childHeritableTraits = mHeritableTraits.GetChildTraits(random);
  Types::HeterotrophPointer childIndividual = 0;

  double childVolumeHeritable = 0;
  double childVolumeActual = 0;
  double childVolumeMinimum = 0;

  if (childHeritableTraits.IsTraitMutant(Constants::eVolume) == false) {
    childVolumeActual = mVolumeActual * Constants::cReproductionMultiplier;
    childVolumeHeritable = mVolumeHeritable;
    childVolumeMinimum = mVolumeMinimum;
  } else {
    childVolumeHeritable = heterotrophProcessor.TraitValueToVolume(childHeritableTraits.GetValue(Constants::eVolume));
    childVolumeMinimum = childVolumeHeritable * Constants::cMinimumFractionalVolume;

    if (childVolumeHeritable < mVolumeActual)
      childVolumeActual = childVolumeHeritable;
    else
      childVolumeActual = mVolumeActual * Constants::cReproductionMultiplier;
  }
  mVolumeActual = mVolumeActual - childVolumeActual;
  childIndividual = new Heterotroph(childHeritableTraits, childVolumeHeritable, childVolumeActual, childVolumeMinimum,
                                    mTrophicLevel, mSizeClassIndex);

  return childIndividual;
}

Heterotroph& Heterotroph::operator=(const Heterotroph& individual) {
  if (this != &individual) {
    mHeritableTraits = individual.mHeritableTraits;

    mVolumeHeritable = individual.mVolumeHeritable;
    mVolumeMinimum = individual.mVolumeMinimum;
    mVolumeReproduction = individual.mVolumeReproduction;

    mVolumeActual = individual.mVolumeActual;
    mTrophicLevel = individual.mTrophicLevel;

    mStarvationMultiplier = individual.mStarvationMultiplier;

    mSizeClassIndex = individual.mSizeClassIndex;
    mAge = individual.mAge;

    mHasFed = individual.mHasFed;
    mIsDead = individual.mIsDead;
  }
  return *this;
}

bool Heterotroph::operator==(const Heterotroph& individual) {
  return (mVolumeActual == individual.mVolumeActual && mVolumeHeritable == individual.mVolumeHeritable);
}

double Heterotroph::ConsumePreyVolume(const double preyVolume) {
  double volumeAssimilated = preyVolume * mAssimilationEfficiency;
  double wasteVolume = preyVolume - volumeAssimilated;

  mVolumeActual += volumeAssimilated;
  mHasFed = true;

  return wasteVolume;
}

double Heterotroph::Metabolise(const double metabolicDeduction) {
  ++mAge;
  mVolumeActual -= metabolicDeduction;
  return metabolicDeduction;
}

HeritableTraits& Heterotroph::GetHeritableTraits() { return mHeritableTraits; }

double Heterotroph::GetTrophicLevel() const { return mTrophicLevel; }

unsigned Heterotroph::GetSizeClassIndex() const { return mSizeClassIndex; }

unsigned Heterotroph::GetAge() const { return mAge; }

bool Heterotroph::HasFed() const { return mHasFed; }

bool Heterotroph::IsDead() const { return mIsDead; }

double Heterotroph::GetVolumeActual() const { return mVolumeActual; }

double Heterotroph::GetVolumeHeritable() const { return mVolumeHeritable; }

double Heterotroph::GetVolumeMinimum() const { return mVolumeMinimum; }

double Heterotroph::GetVolumeReproduction() const { return mVolumeReproduction; }

double Heterotroph::GetStarvationMultiplier() const { return mStarvationMultiplier; }

void Heterotroph::SetTrophicLevel(const double trophicLevel) { mTrophicLevel = trophicLevel; }

void Heterotroph::SetSizeClassIndex(const unsigned sizeClassIndex) { mSizeClassIndex = sizeClassIndex; }

void Heterotroph::SetAge(const unsigned age) { mAge = age; }

void Heterotroph::SetHasFed(const bool hasFed) { mHasFed = hasFed; }

void Heterotroph::Kill() { mIsDead = true; }
