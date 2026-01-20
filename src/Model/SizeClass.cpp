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
std::shared_ptr<Heterotroph> heterotrophGenerator(std::float64_t traitValue,
                                                  std::float64_t volume,
                                                  std::float64_t assimilationEfficiency,
                                            const std::float64_t mutationProbability,
                                            const std::float64_t mutationStandardDeviation) {
  return std::make_shared<Heterotroph>(traitValue, mutationProbability, mutationStandardDeviation,
                                       volume, assimilationEfficiency);
}

std::float64_t volumeToTraitValue(const std::float64_t& volume,
                                  const std::float64_t& smallestVolumeExponent,
                                  const std::float64_t& largestVolumeExponent) {
  return (std::log10(volume) - smallestVolumeExponent) /
         (largestVolumeExponent - smallestVolumeExponent);
}

}  // Anonymous namespace

SizeClass::SizeClass(Nutrient& nutrient, Parameters& params, const std::float64_t initialVolume,
                     const std::uint32_t index, const std::uint32_t randomSeed) :
	nutrient_(nutrient),
        functions_(params),
	index_(index),
        sizeClassUpper_(params.getSizeClassBoundary(index_ + 1)),
        sizeClassMidPoint_(params.getSizeClassMidPoint(index_)),
        sizeClassLower_(params.getSizeClassBoundary(index_)),
        smallestVolumeExponent_(params.getSmallestVolumeExponent()),
        largestVolumeExponent_(params.getLargestVolumeExponent()),
	numberOfSizeClasses_(params.getNumberOfSizeClasses()),
        random_(randomSeed),
        heterotrophs_(nutrient, params, params.getMaximumSizeClassPopulation(index_)) {
  populate(initialVolume, params.getAssimilationEfficiency(),
           params.getMutationProbability(), params.getMutationStandardDeviation());
}

void SizeClass::populate(const std::float64_t volumeToInitialise, const std::float64_t assimilationEfficiency,
                         const std::float64_t mutationProbability, const std::float64_t mutationStandardDeviation) {
  if (volumeToInitialise > 0) {
    std::float64_t realInitialPopulationSize = volumeToInitialise / sizeClassMidPoint_;
    std::uint32_t initialPopulationSize = std::abs(realInitialPopulationSize);
    nutrient_.addToVolume(realInitialPopulationSize - initialPopulationSize);

    std::float64_t traitValue = volumeToTraitValue(sizeClassMidPoint_, smallestVolumeExponent_, largestVolumeExponent_);
    for (auto _ = initialPopulationSize; _--;) {
      std::shared_ptr<Heterotroph> heterotroph = heterotrophGenerator(traitValue, sizeClassMidPoint_, assimilationEfficiency, mutationProbability, mutationStandardDeviation);
      heterotrophs_.addHeterotroph(heterotroph);
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
      nutrient_.addToVolume(waste);
    } else {
      starve(index);
    }
  });
}

void SizeClass::starvation() {
  heterotrophs_.subset(random_, [&](std::uint32_t randomIndex) {
    Heterotroph& heterotroph = heterotrophs_.keyHeterotroph(randomIndex);
    if (random_.getUniform() <= functions_.calcStarvationProbability(heterotroph)) {
      starve(randomIndex);
    }
  });
}

void SizeClass::reproduction() {
  heterotrophs_.forEachHeterotrophIndex([&](std::uint32_t index) {
    Heterotroph& heterotroph = heterotrophs_.keyHeterotroph(index);
    if (heterotroph.getVolumeActual() >= heterotroph.getVolumeReproduction()) {
      std::shared_ptr<Heterotroph> child = heterotroph.getChild(random_, smallestVolumeExponent_, largestVolumeExponent_);
      heterotrophs_.addChild(child);
    }
  });
  heterotrophs_.forEachChild([&](std::shared_ptr<Heterotroph> child) {
    heterotrophs_.addHeterotroph(child);
  });
  heterotrophs_.clearChildren();
}

void SizeClass::whoIsMoving(std::vector<MovingHeterotroph>& movingHeterotrophs) {
  heterotrophs_.forEachHeterotrophIndex([&](std::uint32_t index) {
    std::shared_ptr<Heterotroph> heterotroph = heterotrophs_.ownHeterotroph(index);
    if (heterotroph->getVolumeActual() < sizeClassLower_ && index_ > 0) {  // Zero is smallest size class
      movingHeterotrophs.push_back(MovingHeterotroph(heterotroph, index_, eShrinking));
      heterotrophs_.removeHeterotroph(index);
    } else if (heterotroph->getVolumeActual() >= sizeClassUpper_ && index_ < numberOfSizeClasses_ - 1) {
      movingHeterotrophs.push_back(MovingHeterotroph(heterotroph, index_, eGrowing));
      heterotrophs_.removeHeterotroph(index);
    }
  });
}

void SizeClass::starve(const std::uint32_t index) {
  Heterotroph& heterotroph = heterotrophs_.keyHeterotroph(index);
  nutrient_.addToVolume(heterotroph.getVolumeActual());
  heterotrophs_.removeHeterotroph(index);
}

std::uint32_t SizeClass::getIndex() const {
  return index_;
}

std::uint32_t SizeClass::getPopulationSize() const {
  return heterotrophs_.getLivingCount();
}

std::uint32_t SizeClass::getRandomHeterotrophIndex() {
  return random_.getUniformInt(0, heterotrophs_.getLivingCount() - 1);
}

const Heterotroph& SizeClass::getHeterotroph(const std::uint32_t index) const {
  return heterotrophs_.keyHeterotroph(index);
}

Heterotroph& SizeClass::getHeterotroph(const std::uint32_t index) {
  return heterotrophs_.keyHeterotroph(index);
}

Heterotrophs& SizeClass::getHeterotrophs() {
  return heterotrophs_;
}
