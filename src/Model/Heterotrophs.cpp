/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Heterotrophs.h"

#include <cmath>
#include <stdexcept>

namespace {
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

Heterotrophs::Heterotrophs(Nutrient& nutrient, Parameters& params, EcologicalFunctions& functions,
			   const double& sizeClassMidPoint, const std::uint32_t& maxPopulation,
			   const std::uint32_t& randomSeed):
	nutrient_(nutrient),
	functions_(functions),
	random_(randomSeed),
	sizeClassMidPoint_(sizeClassMidPoint),
	subsetFraction_(params.getSizeClassSubsetFraction()),
	maxPopulation_(maxPopulation) {
  heterotrophs_.reserve(maxPopulation);
  alive_.reserve(maxPopulation);
}

void Heterotrophs::subset(std::function<void(std::uint32_t)> func) {
  std::size_t numberAlive = alive_.size();
  if (numberAlive != 0) {
    std::uint32_t sizeClassSubset = roundWithProbability(random_, numberAlive * subsetFraction_);
    for (auto _ = sizeClassSubset; _--;) {
      func(getRandomHeterotrophIndex());
    }
  }
}

std::size_t Heterotrophs::getPopulationSize() {
  return alive_.size();
}

std::uint32_t Heterotrophs::getRandomHeterotrophIndex() {
  if (alive_.size() != 0) {
    std::uint32_t randomIndex = random_.getUniformInt(alive_.size());
    return alive_[randomIndex];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& Heterotrophs::getRandomHeterotroph() {
  if (alive_.size() != 0) {
    std::uint32_t randomIndex = random_.getUniformInt(alive_.size());
    return heterotrophs_[alive_[randomIndex]];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& Heterotrophs::getRandomHeterotroph(std::uint32_t& randIdxCopy) {
  if (alive_.size() != 0) {
    randIdxCopy = random_.getUniformInt(alive_.size());
    return heterotrophs_[alive_[randIdxCopy]];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& Heterotrophs::getHeterotroph(const std::uint32_t index) {
  if (alive_.size() != 0) {
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

const Heterotroph& Heterotrophs::getHeterotroph(const std::uint32_t index) const {
  if (alive_.size() != 0) {
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

void Heterotrophs::removeHeterotroph(const std::uint32_t index) {
  if (alive_.size() != 0) {
    alive_.erase(std::find(std::begin(alive_), std::end(alive_), index));
    dead_.push(index);
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}


void Heterotrophs::addHeterotroph(Heterotroph heterotroph) {
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

void Heterotrophs::addChild(Heterotroph child) {
  children_.push_back(child);
}


void Heterotrophs::clearChildren() {
  children_.clear();
}

std::vector<Heterotroph>& Heterotrophs::getHeterotrophs() {
  return heterotrophs_;
}

