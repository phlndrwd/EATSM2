#include "SizeClass.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <Parameters.h>

SizeClass::SizeClass(HeterotrophData& heterotrophData,
                     const double sizeClassMidPoint,
                     const unsigned maxPopulation,
                     const unsigned randomSeed) :
    heterotrophData_(heterotrophData),
    random_(randomSeed),
    sizeClassMidPoint_(sizeClassMidPoint) {
  heterotrophs_.reserve(maxPopulation);
  alive_.reserve(maxPopulation);
  //pointer_ = heterotrophs_.begin();

  double individualVolume = sizeClassMidPoint_;
  double traitValue = heterotrophProcessor_.volumeToTraitValue(individualVolume);

  std::vector<double> heritableTraitValues{traitValue};
  std::vector<bool> areTraitsMutant{false};
  Traits heritableTraits(heritableTraitValues, areTraitsMutant);

  for(unsigned index = 0; index < maxPopulation; ++index) {
    dead_.push(index);
    heterotrophs_.push_back(Heterotroph(heritableTraits, individualVolume));
  }
}

std::vector<Heterotroph> SizeClass::update(Nutrient& nutrient) {
  std::vector<Heterotroph> heterotrophsToMove;

  metabolisation(nutrient);

  return heterotrophsToMove;
}

void SizeClass::metabolisation(Nutrient& nutrient) {
  std::for_each(std::begin(alive_), std::end(alive_),
  [&] (unsigned& index)
  {
    Heterotroph& heterotroph = heterotrophs_[index];

    double metabolicDeduction = heterotrophProcessor_.calculateMetabolicDeduction(heterotroph);

    if ((heterotroph.getVolumeActual() - metabolicDeduction) > 0) {
      heterotroph.setHasFed(false);  // Reset for the next time step
      double waste = heterotroph.metabolise(metabolicDeduction);
      nutrient.addToVolume(waste);

      // Individuals can move up a size class from having consumed a
      // lot. They need to move after this function has completed to
      // avoid handling them twice.
//      if (heterotrophProcessor_.updateSizeClassIndex(heterotroph) == true) {
//        heterotrophsToMove.push_back(removeHeterotroph(index));
//      }
    } else {
      starve(heterotroph, nutrient);
    }
  });
}

Heterotroph& SizeClass::getRandomHeterotroph() {
  if(alive_.size() != 0) {
    unsigned randomIndex = random_.getUniformInt(alive_.size());
    return heterotrophs_[alive_[randomIndex]];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& SizeClass::getHeterotroph(const unsigned index) {
  if(alive_.size() != 0) {
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

const Heterotroph& SizeClass::getHeterotroph(const unsigned index) const {
    if(alive_.size() != 0) {
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph SizeClass::removeHeterotroph(const unsigned index) {
  if(alive_.size() != 0) {
    alive_.erase(std::find(std::begin(alive_), std::end(alive_), index));
    dead_.push(index);
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

void SizeClass::addHeterotroph(Heterotroph heterotroph) {
  if(dead_.size() != 0) {
    unsigned index = dead_.front();
    dead_.pop();
    alive_.push_back(index);
    heterotrophs_[index] = std::move(heterotroph);
  } else {
    throw std::runtime_error("Size class is full...");
  }
}

void SizeClass::starve(Heterotroph& heterotroph, Nutrient& nutrient) {
  nutrient.addToVolume(heterotroph.getVolumeActual());
  heterotrophData_.incrementStarvedFrequencies(heterotroph.getSizeClassIndex());
  //kill(heterotroph);
}
