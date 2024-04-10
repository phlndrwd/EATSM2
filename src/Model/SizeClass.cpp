/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "SizeClass.h"

#include <climits>
#include <iostream>

namespace {
Heterotroph heterotrophGenerator(double traitValue, double volume, double assimilationEfficiency,
                                 const double mutationProbability, const double mutationStandardDeviation) {
  std::vector<double> traitValues{traitValue};
  std::vector<std::uint8_t> areTraitsMutant{0};
  Heterotroph heterotroph(traitValues, areTraitsMutant, mutationProbability, mutationStandardDeviation, volume, assimilationEfficiency);
  return heterotroph;
}
}  // Anonymous namespace

SizeClass::SizeClass(Nutrient& nutrient, Parameters& params, EcologicalData& data,
		     const double& initialAutotrophVolume, const double& initialHeterotrophVolume,
		     const std::uint32_t& index, const std::uint32_t& randomSeed):
	nutrient_(nutrient),
	functions_(data, params),
	index_(index),
	sizeClassUpper_(data.getSizeClassBoundaries()[index_ + 1]),
	sizeClassMidPoint_(data.getSizeClassMidPoints()[index_]),
	sizeClassLower_(data.getSizeClassBoundaries()[index_]),
	numberOfSizeClasses_(params.getNumberOfSizeClasses()),
	random_(randomSeed),
	autotrophs_(nutrient, initialAutotrophVolume),
	heterotrophs_(nutrient, params, functions_, sizeClassMidPoint_,
		      data.getMaximumSizeClassPopulations()[index_], random_.getUniformInt(1, UINT_MAX)) {
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
    for (auto _ = initialPopulationSize; _--;) {
      Heterotroph heterotroph = heterotrophGenerator(traitValue, sizeClassMidPoint_, assimilationEfficiency,
                                                     mutationProbability, mutationStandardDeviation);
      heterotrophs_.addHeterotroph(std::move(heterotroph));
    }
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
  heterotrophs_.forEachHeterotrophIndex([&](std::uint32_t index) {
    Heterotroph& heterotroph = heterotrophs_.getHeterotroph(index);
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
  heterotrophs_.subset([&](std::uint32_t randomIndex) {
    Heterotroph& heterotroph = heterotrophs_.getHeterotroph(randomIndex);
    if (random_.getUniform() <= functions_.calcStarvationProbability(heterotroph)) {
      starve(randomIndex);
    }
  });
}

void SizeClass::reproduction() {
  heterotrophs_.forEachHeterotrophIndex([&](std::uint32_t index) {
    Heterotroph& heterotroph = heterotrophs_.getHeterotroph(index);
    if (heterotroph.getVolumeActual() >= heterotroph.getVolumeReproduction()) {
      Heterotroph child = heterotroph.getChild(random_, functions_);
      heterotrophs_.addChild(child);
    }
  });
  heterotrophs_.forEachChild([&](Heterotroph& child) {
    heterotrophs_.addHeterotroph(child);
  });
  heterotrophs_.clearChildren();
}

void SizeClass::moveSizeClass(std::vector<structs::MovingHeterotroph>& movingHeterotrophs) {
  heterotrophs_.forEachHeterotrophIndex([&](std::uint32_t index) {
    Heterotroph& heterotroph = heterotrophs_.getHeterotroph(index);
    heterotrophs_.removeHeterotroph(index);
    if (heterotroph.getVolumeActual() < sizeClassLower_ && index_ > 0) {  // Zero is smallest size class
      movingHeterotrophs.push_back(structs::MovingHeterotroph(heterotroph, index_, enums::eMoveDown));
    } else if (heterotroph.getVolumeActual() >= sizeClassUpper_ && index_ < numberOfSizeClasses_ - 1) {
      movingHeterotrophs.push_back(structs::MovingHeterotroph(heterotroph, index_, enums::eMoveUp));
    }
  });
}

void SizeClass::starve(const std::uint32_t index) {
  Heterotroph& heterotroph = heterotrophs_.getHeterotroph(index);
  nutrient_.addToVolume(heterotroph.getVolumeActual());
  heterotrophs_.removeHeterotroph(index);
  //heterotrophData_.incrementStarvedFrequencies(heterotroph.getSizeClassIndex());
}

Autotrophs& SizeClass::getAutotrophs() {
  return autotrophs_;
}

Heterotrophs& SizeClass::getHeterotrophs() {
  return heterotrophs_;
}

std::uint32_t SizeClass::getIndex() const {
  return index_;
}
