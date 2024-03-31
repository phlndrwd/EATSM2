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
  std::vector<unsigned char> areTraitsMutant{0};
  Heterotroph heterotroph(traitValues, areTraitsMutant, mutationProbability, mutationStandardDeviation, volume, assimilationEfficiency);
  return heterotroph;
}

int roundWithProbability(RandomSimple& random, const double value) {
  int flooredValue = static_cast<int>(std::floor(value));
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
		     const unsigned& index, const unsigned& randomSeed):
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
    unsigned initialPopulationSize = std::abs(realInitialPopulationSize);
    nutrient_.addToVolume(realInitialPopulationSize - initialPopulationSize);

    double traitValue = functions_.volumeToTraitValue(sizeClassMidPoint_);
    unsigned heterotrophIndex = 0;
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
  metabolisation();
  starvation();
  reproduction(); // Not implemented
  move(movingHeterotrophs);
}

void SizeClass::metabolisation() {
  std::for_each(std::begin(alive_), std::end(alive_), [&](unsigned index) {
    Heterotroph& heterotroph = heterotrophs_[index];
    double metabolicDeduction = functions_.calculateMetabolicDeduction(heterotroph);

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
  sizeClassSubset([&](unsigned randomIndex) {
    Heterotroph& heterotroph = heterotrophs_[randomIndex];
    if (random_.getUniform() <= functions_.calculateStarvationProbability(heterotroph)) {
      starve(randomIndex);
    }
  });
}

void SizeClass::reproduction() {

}

void SizeClass::move(std::vector<structs::MovingHeterotroph>& movingHeterotrophs) {
  std::for_each(std::begin(alive_), std::end(alive_), [&](unsigned index) {
    Heterotroph& heterotroph = heterotrophs_[index];
    if (heterotroph.getVolumeActual() < sizeClassLower_ && index_ > 0) {  // Zero is smallest
      removeHeterotroph(index);
      movingHeterotrophs.push_back(structs::MovingHeterotroph(heterotroph, enums::eMoveDown));
    } else if (heterotroph.getVolumeActual() >= sizeClassUpper_ && index_ < numberOfSizeClasses_ - 1) {  // Max is high
      removeHeterotroph(index);
      movingHeterotrophs.push_back(structs::MovingHeterotroph(heterotroph, enums::eMoveUp));
    }
  });
}

void SizeClass::sizeClassSubset(std::function<void(unsigned)> func) {
  std::size_t numberAlive = alive_.size();
  if (numberAlive != 0) {
    unsigned sizeClassSubset = roundWithProbability(random_, numberAlive * sizeClassSubsetFraction_);
    for (auto _ = sizeClassSubset; _--;) {
      func(getRandomHeterotrophIndex());
    }
  }
}

void SizeClass::starve(const unsigned index) {
  Heterotroph& heterotroph = heterotrophs_[index];
  nutrient_.addToVolume(heterotroph.getVolumeActual());
  removeHeterotroph(index);
  //heterotrophData_.incrementStarvedFrequencies(heterotroph.getSizeClassIndex());
}

std::size_t SizeClass::getPopulationSize() {
  return alive_.size();
}

unsigned SizeClass::getRandomHeterotrophIndex() {
  if (alive_.size() != 0) {
    unsigned randomIndex = random_.getUniformInt(alive_.size());
    return alive_[randomIndex];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& SizeClass::getRandomHeterotroph() {
  if (alive_.size() != 0) {
    unsigned randomIndex = random_.getUniformInt(alive_.size());
    return heterotrophs_[alive_[randomIndex]];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& SizeClass::getHeterotroph(const unsigned index) {
  if (alive_.size() != 0) {
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

const Heterotroph& SizeClass::getHeterotroph(const unsigned index) const {
  if (alive_.size() != 0) {
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

void SizeClass::removeHeterotroph(const unsigned index) {
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
    int index;
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

unsigned SizeClass::getIndex() const {
  return index_;
}
