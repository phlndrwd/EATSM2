/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "HeterotrophPopulation.h"

#include <cmath>
#include <climits>
#include <memory>
#include <iostream>

namespace {


std::uint32_t Life::findSizeClassIndexFromVolume(const std::float64_t& volume) const {
  std::uint32_t sizeClassIndex = 0;
  for (std::uint32_t index = 1; index <= numberOfSizeClasses_; ++index) {
    if (volume < params->getSizeClassBoundary(index)) {
      sizeClassIndex = index - 1;
      break;
    }
  }
  return sizeClassIndex;
}

std::float64_t volumeToTraitValue(const std::float64_t& volume,
                                  const std::float64_t& smallestVolumeExponent,
                                  const std::float64_t& largestVolumeExponent) {
  return (std::log10(volume) - smallestVolumeExponent) /
         (largestVolumeExponent - smallestVolumeExponent);
}


std::unique_ptr<Heterotroph> heterotrophGenerator(std::float64_t traitValue,
                                                  std::float64_t volume,
                                                  std::float64_t assimilationEfficiency,
                                            const std::float64_t mutationProbability,
                                            const std::float64_t mutationStandardDeviation) {
  return std::make_unique<Heterotroph>(traitValue, mutationProbability, mutationStandardDeviation, volume, assimilationEfficiency);
}



}  // Anonymous namespace

HeterotrophPopulation::HeterotrophPopulation(Nutrient* nutrient, Parameters* params, const std::uint32_t randomSeed) :
	nutrient_(nutrient),
        functions_(*params),
        random_(randomSeed),
        deadIndices_(),
        numberOfSizeClasses_(params->getNumberOfSizeClasses()) {
  populate(Parameters* params);
}

void HeterotrophPopulation::update() {
   metabolisation();
   starvation();
   reproduction();
}

void HeterotrophPopulation::populate(Parameters* params) {
  sizeClasses_.reserve(numberOfSizeClasses_);
  const std::float64_t initialVolume = params->getInitialHeterotrophVolume();

  std::uint32_t heterotrophIndex = findSizeClassIndexFromVolume(params->getIndividualHeterotrophVolume());
  for (std::uint32_t index = 0; index < numberOfSizeClasses_; ++index) {

    sizeClasses_.emplace_back(params, index);  // Initialise SizeClass

    if (initialVolume > 0 && index == heterotrophIndex) {  // Populate single SizeClass
      std::float64_t sizeClassMidPoint = sizeClasses_[index].getSizeClassMidPoint();
      std::float64_t realInitialPopulationSize = initialVolume / sizeClassMidPoint;
      std::uint32_t initialPopulationSize = static_cast<std::uint32_t>(std::floor(realInitialPopulationSize));
      std::float64_t unusedVolume = (realInitialPopulationSize - initialPopulationSize) * sizeClassMidPoint;

      nutrient_->addToVolume(unusedVolume); // DEBUG AND CHECK THE CALCULATION HERE //////////////////////////////////////

      std::float64_t traitValue = volumeToTraitValue(
        sizeClassMidPoint,
        params->getSmallestVolumeExponent(),
        params->getLargestVolumeExponent()
      );

      for (auto _ = initialPopulationSize; _--;) {
        sizeClasses_[index].addHeterotroph(
          heterotrophGenerator(
            traitValue,
            sizeClassMidPoint,
            params->getAssimilationEfficiency(),
            params->getMutationProbability(),
            params->getMutationStandardDeviation()
          )
        );
      }
      std::cout << "Size class with index " << index << " initialised with " << initialPopulationSize << " heterotrophs." << std::endl;
    }
  }
}

void HeterotrophPopulation::metabolisation() {
  for (auto& sizeClass : sizeClasses_ ) {
    sizeClass.forEachHeterotroph(
      [&](Heterotroph& heterotroph) {
        std::float64_t metabolicDeduction = functions_.calcMetabolicDeduction(heterotroph);
        if ((heterotroph.getVolumeActual() - metabolicDeduction) > 0) {
          std::float64_t waste = heterotroph.metabolise(metabolicDeduction);
          nutrient_->addToVolume(waste);
        } else {
          nutrient_->addToVolume(heterotroph.getVolumeActual());

          sizeClass.
          deadIndices_.push_back(index);
        }
      }
    );
  }
  removeDead();
}

void HeterotrophPopulation::starvation() {
  for (auto& sizeClass : sizeClasses_ ) {
    sizeClass.forEachHeterotroph(
      [&](std::uint32_t randomIndex, Heterotroph& heterotroph) {
        if (random_.getUniform() <= functions_.calcStarvationProbability(heterotroph)) {
          nutrient_->addToVolume(heterotroph.getVolumeActual());
          deadIndices_.push_back(randomIndex);
        }
      }
    );
  }
  removeDead();
}

void HeterotrophPopulation::reproduction() {
  for (auto& sizeClass : sizeClasses_ ) {
    sizeClass.forEachHeterotroph(
      [&](std::uint32_t index, Heterotroph& heterotroph) {
        if (heterotroph.getVolumeActual() >= heterotroph.getVolumeReproduction()) {
          sizeClasses.addChild(
            heterotroph.getChild(random_, smallestVolumeExponent_, largestVolumeExponent_)
          );
        }

        if (heterotroph.getVolumeActual() < sizeClassLower_ && index_ > 0) {  // Zero is smallest size class
          movingHeterotrophs.push_back(MovingHeterotroph(sizeClasses_.ownHeterotroph(index),
                                       index_, eShrinking));
          killHeterotroph(currentIndex);
        } else if (heterotroph.getVolumeActual() >= sizeClassUpper_ && index_ < numberOfSizeClasses_ - 1) {
          movingHeterotrophs.push_back(MovingHeterotroph(sizeClasses_.ownHeterotroph(index),
                                       index_, eGrowing));
          killHeterotroph(currentIndex);
        }
      }
    );
  }
  removeDead();
  sizeClasses_.forEachChild([&](std::unique_ptr<Heterotroph> child) {
    sizeClasses_.addHeterotroph(std::move(child));
  });
  sizeClasses_.clearChildren();
}

void HeterotrophPopulation::starve(const std::uint32_t index) {
  Heterotroph& heterotroph = sizeClasses_.keyHeterotroph(index);
  nutrient_->addToVolume(heterotroph.getVolumeActual());
  deadIndices_.push_back(index);
}

std::uint32_t HeterotrophPopulation::getIndex() const {
  return index_;
}

std::uint32_t HeterotrophPopulation::getPopulationSize() const {
  return sizeClasses_.getLivingCount();
}

const Heterotroph& HeterotrophPopulation::getHeterotroph(const std::uint32_t index) const {
  return sizeClasses_.keyHeterotroph(index);
}

Heterotroph& HeterotrophPopulation::getHeterotroph(const std::uint32_t index) {
  return sizeClasses_.keyHeterotroph(index);
}

Heterotroph& HeterotrophPopulation::getRandomHeterotroph(std::uint32_t& livingIndex) {
  std::uint32_t randomIndex = random_.getUniformInt(sizeClasses_.getLivingCount() - 1);
  livingIndex = sizeClasses_.getLivingIndex(randomIndex);
  return sizeClasses_.keyHeterotroph(livingIndex);
}

std::uint32_t HeterotrophPopulation::getLivingIndex(const std::uint32_t index) {
  return sizeClasses_.getLivingIndex(index);
}

SizeClass& HeterotrophPopulation::getHeterotrophs() {
  return sizeClasses_;
}

std::float64_t HeterotrophPopulation::getVolume() const {
  std::float64_t volume = 0;
  sizeClasses_.forEachHeterotrophIndex([&](std::uint32_t index) {
    const Heterotroph& heterotroph = sizeClasses_.keyHeterotroph(index);
    volume += heterotroph.getVolumeActual();
  });
  return volume;
}

void HeterotrophPopulation::addHeterotroph(std::unique_ptr<Heterotroph> heterotroph) {
  sizeClasses_.addHeterotroph(std::move(heterotroph));
}

void HeterotrophPopulation::killHeterotroph(const std::uint32_t index) {
  deadIndices_.push_back(index);
}

void HeterotrophPopulation::removeDead() {
  for (const auto index : deadIndices_) {
    sizeClasses_.removeHeterotroph(index);
  }
  deadIndices_.clear();
}
