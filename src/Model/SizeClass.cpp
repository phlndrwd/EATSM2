/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "SizeClass.h"

#include <cmath>
#include <climits>
#include <memory>
#include <iostream>

namespace {
std::unique_ptr<Heterotroph> heterotrophGenerator(std::float64_t traitValue,
                                                  std::float64_t volume,
                                                  std::float64_t assimilationEfficiency,
                                            const std::float64_t mutationProbability,
                                            const std::float64_t mutationStandardDeviation) {
  return std::make_unique<Heterotroph>(traitValue, mutationProbability, mutationStandardDeviation, volume, assimilationEfficiency);
}

std::float64_t volumeToTraitValue(const std::float64_t& volume,
                                  const std::float64_t& smallestVolumeExponent,
                                  const std::float64_t& largestVolumeExponent) {
  return (std::log10(volume) - smallestVolumeExponent) /
         (largestVolumeExponent - smallestVolumeExponent);
}

}  // Anonymous namespace

SizeClass::SizeClass(Nutrient* nutrient, Parameters* params, const std::float64_t initialVolume,
                     const std::uint32_t index, const std::uint32_t randomSeed) :
	nutrient_(nutrient),
        functions_(*params),
        random_(randomSeed),
        heterotrophs_(*nutrient, *params, params->getMaximumSizeClassPopulation(index)),
        deadIndices_(),
        sizeClassUpper_(params->getSizeClassBoundary(index + 1)),
        sizeClassMidPoint_(params->getSizeClassMidPoint(index)),
        sizeClassLower_(params->getSizeClassBoundary(index)),
        smallestVolumeExponent_(params->getSmallestVolumeExponent()),
        largestVolumeExponent_(params->getLargestVolumeExponent()),
        index_(index),
        numberOfSizeClasses_(params->getNumberOfSizeClasses()) {
  populate(initialVolume, params->getAssimilationEfficiency(),
           params->getMutationProbability(), params->getMutationStandardDeviation());
}

void SizeClass::populate(const std::float64_t volumeToInitialise, const std::float64_t assimilationEfficiency,
                         const std::float64_t mutationProbability, const std::float64_t mutationStandardDeviation) {
  if (volumeToInitialise > 0) {
    std::float64_t realInitialPopulationSize = volumeToInitialise / sizeClassMidPoint_;
    std::uint32_t initialPopulationSize = std::abs(realInitialPopulationSize);
    nutrient_->addToVolume(realInitialPopulationSize - initialPopulationSize);

    std::float64_t traitValue = volumeToTraitValue(sizeClassMidPoint_, smallestVolumeExponent_, largestVolumeExponent_);
    for (auto _ = initialPopulationSize; _--;) {
      heterotrophs_.addHeterotroph(heterotrophGenerator(traitValue, sizeClassMidPoint_, assimilationEfficiency, mutationProbability, mutationStandardDeviation));
    }
    std::cout << "Size class with index " << index_ << " initialised with " << initialPopulationSize <<
                 " heterotrophs." << std::endl;
  }
}

void SizeClass::metabolisation() {
  heterotrophs_.forEachHeterotrophIndex([&](const std::uint32_t index) {
    Heterotroph& heterotroph = heterotrophs_.keyHeterotroph(index);
    std::float64_t metabolicDeduction = functions_.calcMetabolicDeduction(heterotroph);
    if ((heterotroph.getVolumeActual() - metabolicDeduction) > 0) {
      std::float64_t waste = heterotroph.metabolise(metabolicDeduction);
      nutrient_->addToVolume(waste);
    } else {
      //starve(index);
      nutrient_->addToVolume(heterotroph.getVolumeActual());
      deadIndices_.push_back(index);
    }
  });
  removeDead();
}

void SizeClass::starvation() {
  heterotrophs_.subset(random_, [&](std::uint32_t randomIndex) {
    Heterotroph& heterotroph = heterotrophs_.keyHeterotroph(randomIndex);
    if (random_.getUniform() <= functions_.calcStarvationProbability(heterotroph)) {
      //starve(randomIndex);
      nutrient_->addToVolume(heterotroph.getVolumeActual());
      deadIndices_.push_back(randomIndex);
    }
  });
  removeDead();
}

void SizeClass::reproduction() {
  heterotrophs_.forEachHeterotrophIndex([&](std::uint32_t index) {
    Heterotroph& heterotroph = heterotrophs_.keyHeterotroph(index);
    if (heterotroph.getVolumeActual() >= heterotroph.getVolumeReproduction()) {
      heterotrophs_.addChild(
        heterotroph.getChild(random_, smallestVolumeExponent_, largestVolumeExponent_)
      );
    }
  });
  heterotrophs_.forEachChild([&](std::unique_ptr<Heterotroph> child) {
    heterotrophs_.addHeterotroph(std::move(child));
  });
  heterotrophs_.clearChildren();
}

void SizeClass::whoIsMoving(std::vector<MovingHeterotroph>& movingHeterotrophs) {
  heterotrophs_.forEachHeterotrophIndex([&](std::uint32_t currentIndex) {
    Heterotroph& heterotroph = heterotrophs_.keyHeterotroph(currentIndex);
    if (heterotroph.getVolumeActual() < sizeClassLower_ && index_ > 0) {  // Zero is smallest size class
      movingHeterotrophs.push_back(MovingHeterotroph(heterotrophs_.ownHeterotroph(currentIndex),
                                   currentIndex, index_, eShrinking));
    } else if (heterotroph.getVolumeActual() >= sizeClassUpper_ && index_ < numberOfSizeClasses_ - 1) {
      movingHeterotrophs.push_back(MovingHeterotroph(heterotrophs_.ownHeterotroph(currentIndex),
                                   currentIndex, index_, eGrowing));
    }
  });
  //removeDead();
}

void SizeClass::starve(const std::uint32_t index) {
  Heterotroph& heterotroph = heterotrophs_.keyHeterotroph(index);
  nutrient_->addToVolume(heterotroph.getVolumeActual());
  deadIndices_.push_back(index);
}

std::uint32_t SizeClass::getIndex() const {
  return index_;
}

std::uint32_t SizeClass::getPopulationSize() const {
  return heterotrophs_.getLivingCount();
}

const Heterotroph& SizeClass::getHeterotroph(const std::uint32_t index) const {
  return heterotrophs_.keyHeterotroph(index);
}

Heterotroph& SizeClass::getHeterotroph(const std::uint32_t index) {
  return heterotrophs_.keyHeterotroph(index);
}

Heterotroph& SizeClass::getRandomHeterotroph(std::uint32_t& livingIndex) {
  std::uint32_t randomIndex = random_.getUniformInt(heterotrophs_.getLivingCount() - 1);
  livingIndex = heterotrophs_.getLivingIndex(randomIndex);
  return heterotrophs_.keyHeterotroph(livingIndex);
}

std::uint32_t SizeClass::getLivingIndex(const std::uint32_t index) {
  return heterotrophs_.getLivingIndex(index);
}

Heterotrophs& SizeClass::getHeterotrophs() {
  return heterotrophs_;
}

std::float64_t SizeClass::getVolume() const {
  std::float64_t volume = 0;
  heterotrophs_.forEachHeterotrophIndex([&](std::uint32_t index) {
    const Heterotroph& heterotroph = heterotrophs_.keyHeterotroph(index);
    volume += heterotroph.getVolumeActual();
  });
  return volume;
}

void SizeClass::addHeterotroph(std::unique_ptr<Heterotroph> heterotroph) {
  heterotrophs_.addHeterotroph(std::move(heterotroph));
}

void SizeClass::killHeterotroph(const std::uint32_t index) {
  deadIndices_.push_back(index);
}

void SizeClass::removeDead() {
  for (const auto index : deadIndices_) {
    heterotrophs_.removeHeterotroph(index);
  }
  deadIndices_.clear();
}

std::float64_t SizeClass::getSizeClassUpper() {
  return sizeClassUpper_;
}

std::float64_t SizeClass::getSizeClassMidPoint() {
  return sizeClassMidPoint_;
}

std::float64_t SizeClass::getSizeClassLower() {
  return sizeClassLower_;
}
