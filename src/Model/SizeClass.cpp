#include "SizeClass.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <Parameters.h>

SizeClass::SizeClass(RandomSimple& random, const double sizeClassMidPoint, const unsigned maxPopulation) :
    random_(random),
    sizeClassMidPoint_(sizeClassMidPoint),
    maxPopulation_(maxPopulation)
{
  heterotrophs_.reserve(maxPopulation);
  pointer_ = heterotrophs_.begin();

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

void SizeClass::update() {
  std::for_each(std::begin(alive_), std::end(alive_),
  [&] (unsigned& index)
  {
    Heterotroph& heterotroph = heterotrophs_[index];
    heterotroph.setDead();
  });}

Heterotroph& SizeClass::getRandomHeterotroph() {
  if(alive_.size() != 0) {
    unsigned randomIndex = random_.getUniformInt(maxPopulation_);
    return heterotrophs_[randomIndex];
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
    dead_.push(index);


    //alive_.find_and_remove(index);
    return std::move(heterotrophs_[index]);
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
