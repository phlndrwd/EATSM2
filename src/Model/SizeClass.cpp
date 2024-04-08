/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "SizeClass.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace {
Heterotroph heterotrophGenerator(double traitValue, double volume, double assimilationEfficiency,
                                 const double mutationProbability, const double mutationStandardDeviation) {
  std::vector<double> traitValues{traitValue};
  std::vector<std::uint8_t> areTraitsMutant{0};
  Heterotroph heterotroph(traitValues, areTraitsMutant, mutationProbability, mutationStandardDeviation, volume, assimilationEfficiency);
  return heterotroph;
}

std::int32_t roundWithProbability(RandomSimple& random, const double value) {
  std::int32_t flooredValue = static_cast<std::int32_t>(std::floor(value));
  double probability = value - flooredValue;

  if (random.getUniform() < probability) {
    return flooredValue + 1;
  } else {
    return flooredValue;
  }
}
}  // anonymous namespace

SizeClass::SizeClass(Nutrient& nutrient, Parameters& params, EcologicalData& data, EcologicalFunctions& functions,
		     const double& initialAutotrophVolume, const double& initialHeterotrophVolume,
		     const std::uint32_t& index, const std::uint32_t& randomSeed):
	nutrient_(nutrient),
	functions_(functions),
	index_(index),
	sizeClassUpper_(data.getSizeClassBoundaries()[index_ + 1]),
	sizeClassMidPoint_(data.getSizeClassMidPoints()[index_]),
	sizeClassLower_(data.getSizeClassBoundaries()[index_]),
	sizeClassSubsetFraction_(params.getSizeClassSubsetFraction()),
	numberOfSizeClasses_(params.getNumberOfSizeClasses()),
	maxPopulation_(data.getMaximumSizeClassPopulations()[index_]),
	autotrophs_(nutrient, initialAutotrophVolume),
	random_(randomSeed) {
  heterotrophs_.reserve(maxPopulation_);
  alive_.reserve(maxPopulation_);
  populate(initialHeterotrophVolume, params.getAssimilationEfficiency(),
           params.getMutationProbability(), params.getMutationStandardDeviation());
}

void SizeClass::populate(const double volumeToInitialise, const double assimilationEfficiency,
                         const double mutationProbability, const double mutationStandardDeviation) {
  if (volumeToInitialise > 0) {
    double realInitialPopulationSize = volumeToInitialise / sizeClassMidPoint_;
    std::uint32_t initialPopulationSize = std::abs(realInitialPopulationSize);
    nutrient_.addToVolume(realInitialPopulationSize - initialPopulationSize);

    double traitValue = functions_.volumeToTraitValue(sizeClassMidPoint_);
    std::uint32_t heterotrophIndex = 0;
    std::generate_n(std::back_inserter(heterotrophs_), initialPopulationSize, [&] {
      alive_.push_back(heterotrophIndex);
      ++heterotrophIndex;
      return heterotrophGenerator(traitValue, sizeClassMidPoint_, assimilationEfficiency, mutationProbability, mutationStandardDeviation);
    });
    std::cout << "Size class with index " << index_ << " initialised with " << initialPopulationSize <<
                 " heterotrophs." << std::endl;
  }
}

void SizeClass::update(std::vector<structs::MovingHeterotroph>& movingHeterotrophs) {
  movingHeterotrophs.clear();
  metabolisation();
  starvation();
  reproduction(); // Not implemented
  moveSizeClass(movingHeterotrophs);
}

void SizeClass::metabolisation() {
  std::for_each(std::begin(alive_), std::end(alive_), [&](std::uint32_t index) {
    Heterotroph& heterotroph = heterotrophs_[index];
    double metabolicDeduction = functions_.calcMetabolicDeduction(heterotroph);

    if ((heterotroph.getVolumeActual() - metabolicDeduction) > 0) {
      heterotroph.setHasFed(false);  // Reset for the next time step
      double waste = heterotroph.metabolise(metabolicDeduction);
      nutrient_.addToVolume(waste);
    } else {
      starve(index);
    }
  });
}

void SizeClass::starvation() {
  sizeClassSubset([&](std::uint32_t randomIndex) {
    Heterotroph& heterotroph = heterotrophs_[randomIndex];
    if (random_.getUniform() <= functions_.calcStarvationProbability(heterotroph)) {
      starve(randomIndex);
    }
  });
}

void SizeClass::reproduction() {

}

void SizeClass::moveSizeClass(std::vector<structs::MovingHeterotroph>& movingHeterotrophs) {
  std::for_each(std::begin(alive_), std::end(alive_), [&](std::uint32_t index) {
    Heterotroph& heterotroph = heterotrophs_[index];
    removeHeterotroph(index);
    if (heterotroph.getVolumeActual() < sizeClassLower_ && index_ > 0) {  // Zero is smallest
      movingHeterotrophs.push_back(structs::MovingHeterotroph(heterotroph, index_, enums::eMoveDown));
    } else if (heterotroph.getVolumeActual() >= sizeClassUpper_ && index_ < numberOfSizeClasses_ - 1) {  // Max is high
      movingHeterotrophs.push_back(structs::MovingHeterotroph(heterotroph, index_, enums::eMoveUp));
    }
  });
}

void SizeClass::sizeClassSubset(std::function<void(std::uint32_t)> func) {
  std::size_t numberAlive = alive_.size();
  if (numberAlive != 0) {
    std::uint32_t sizeClassSubset = roundWithProbability(random_, numberAlive * sizeClassSubsetFraction_);
    for (auto _ = sizeClassSubset; _--;) {
      func(getRandomHeterotrophIndex());
    }
  }
}

void SizeClass::starve(const std::uint32_t index) {
  Heterotroph& heterotroph = heterotrophs_[index];
  nutrient_.addToVolume(heterotroph.getVolumeActual());
  removeHeterotroph(index);
  //heterotrophData_.incrementStarvedFrequencies(heterotroph.getSizeClassIndex());
}

std::size_t SizeClass::getPopulationSize() {
  return alive_.size();
}

std::uint32_t SizeClass::getRandomHeterotrophIndex() {
  if (alive_.size() != 0) {
    std::uint32_t randomIndex = random_.getUniformInt(alive_.size());
    return alive_[randomIndex];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& SizeClass::getRandomHeterotroph() {
  if (alive_.size() != 0) {
    std::uint32_t randomIndex = random_.getUniformInt(alive_.size());
    return heterotrophs_[alive_[randomIndex]];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& SizeClass::getRandomHeterotroph(std::uint32_t& randIdxCopy) {
  if (alive_.size() != 0) {
    randIdxCopy = random_.getUniformInt(alive_.size());
    return heterotrophs_[alive_[randIdxCopy]];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& SizeClass::getHeterotroph(const std::uint32_t index) {
  if (alive_.size() != 0) {
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

const Heterotroph& SizeClass::getHeterotroph(const std::uint32_t index) const {
  if (alive_.size() != 0) {
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

void SizeClass::removeHeterotroph(const std::uint32_t index) {
  if (alive_.size() != 0) {
    alive_.erase(std::find(std::begin(alive_), std::end(alive_), index));
    dead_.push(index);
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Autotrophs& SizeClass::getAutotrophs() {
  return autotrophs_;
}

void SizeClass::addHeterotroph(Heterotroph heterotroph) {
  if (alive_.size() != maxPopulation_) {
    std::int32_t index;
    std::vector<Heterotroph>::iterator heterotrophsIt;
    if (dead_.size() != 0) {
      index = dead_.front();
      dead_.pop();
      heterotrophsIt = heterotrophs_.begin();
      std::advance(heterotrophsIt, index);
    } else {
      index = heterotrophs_.size();
      heterotrophsIt = heterotrophs_.end();
    }
    heterotrophs_.insert(heterotrophsIt, std::move(heterotroph));
    alive_.push_back(index);
  } else {
    throw std::runtime_error("Size class is full...");
  }
}

std::uint32_t SizeClass::getIndex() const {
  return index_;
}
