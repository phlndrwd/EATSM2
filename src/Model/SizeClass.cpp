#include "SizeClass.h"

#include <algorithm>
#include <stdexcept>

SizeClass::SizeClass(RandomSimple& random, const unsigned maxPopulation) :
    random_(random),
    maxPopulation_(maxPopulation)
{
  heterotrophs_.reserve(maxPopulation);
  pointer_ = heterotrophs_.begin();
  for(unsigned index = 0; index < maxPopulation; ++index) {
    dead_.push(index);
  }
}

void SizeClass::update() {
  std::for_each(std::begin(heterotrophs_), std::end(heterotrophs_),
  [] (Heterotroph& heterotroph)
  {
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
    alive_.push(index);
    return std::move(heterotrophs_[index]);
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

void SizeClass::addHeterotroph(Heterotroph heterotroph) {
  if(dead_.size() != 0) {
    unsigned index = dead_.front();
    dead_.pop();
    alive_.push(index);
    heterotrophs_[index] = std::move(heterotroph);
  } else {
    throw std::runtime_error("Size class is full...");
  }
}
