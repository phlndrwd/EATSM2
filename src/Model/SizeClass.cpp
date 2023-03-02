#include "SizeClass.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <Parameters.h>

SizeClass::SizeClass(HeterotrophData& heterotrophData,
                     const double sizeClassMidPoint,
                     const double sizeClassSubsetFraction,
                     const unsigned maxPopulation,
                     const unsigned randomSeed) :
    heterotrophData_(heterotrophData),
    random_(randomSeed),
    sizeClassMidPoint_(sizeClassMidPoint),
    sizeClassSubsetFraction_(sizeClassSubsetFraction) {
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
    heterotrophs_[index];

    double metabolicDeduction = heterotrophProcessor_.calculateMetabolicDeduction(heterotrophs_[index]);

    if ((heterotrophs_[index].getVolumeActual() - metabolicDeduction) > 0) {
      heterotrophs_[index].setHasFed(false);  // Reset for the next time step
      double waste = heterotrophs_[index].metabolise(metabolicDeduction);
      nutrient.addToVolume(waste);
    } else {
      starve(nutrient, index);
    }
  });
}

void SizeClass::starvation(Nutrient& nutrient) {
  std::size_t numberAlive = alive_.size();

  if (numberAlive > 0) {
    unsigned populationSubset = heterotrophProcessor_.roundWithProbability(random_, numberAlive * sizeClassSubsetFraction_);

    for (unsigned potentialStarvation = 0; potentialStarvation < populationSubset; ++potentialStarvation) {
      unsigned randomHeterotrophIndex = getRandomHeterotrophIndex();

      if (random_.getUniform() <= heterotrophProcessor_.calculateStarvationProbability(heterotrophs_[randomHeterotrophIndex])) {
        starve(nutrient, randomHeterotrophIndex);
      }
    }
  }
}

unsigned SizeClass::getRandomHeterotrophIndex() {
  if(alive_.size() != 0) {
    unsigned randomIndex = random_.getUniformInt(alive_.size());
    return alive_[randomIndex];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
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

void SizeClass::starve(Nutrient& nutrient, const unsigned index) {
  Heterotroph heterotroph = removeHeterotroph(index);
  nutrient.addToVolume(heterotroph.getVolumeActual());
  heterotrophData_.incrementStarvedFrequencies(heterotroph.getSizeClassIndex());
}
