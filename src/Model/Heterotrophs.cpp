/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Heterotrophs.h"

#include <stdexcept>

Heterotrophs::Heterotrophs(Nutrient& nutrient,
                            Parameters& params,
                     const std::uint32_t maxPopulation) :
        nutrient_(nutrient),
	subsetFraction_(params.getSizeClassSubsetFraction()),
	maxPopulation_(maxPopulation) {
  heterotrophs_.reserve(maxPopulation);
  alive_.reserve(maxPopulation);
}

Heterotroph& Heterotrophs::keyHeterotroph(const std::uint32_t index) {
  if (alive_.size() != 0) {
    return *heterotrophs_.at(index).get();
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

const Heterotroph& Heterotrophs::keyHeterotroph(const std::uint32_t index) const {
  if (alive_.size() != 0) {
    return *heterotrophs_.at(index).get();
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

std::unique_ptr<Heterotroph> Heterotrophs::ownHeterotroph(const std::uint32_t index) {
  if (alive_.size() != 0) {
      return std::move(heterotrophs_.at(index));
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

void Heterotrophs::addHeterotroph(std::unique_ptr<Heterotroph> heterotroph) {
  if (alive_.size() != maxPopulation_) {
    std::int32_t index;
    if (dead_.size() != 0) {
      index = dead_.front();
      dead_.pop();
    } else {
      index = heterotrophs_.size();
    }
    auto heterotrophsIt = std::next(heterotrophs_.begin(), index);
    heterotrophs_.insert(heterotrophsIt, std::move(heterotroph));
    alive_.push_back(index);
  } else {
    throw std::runtime_error("Size class is full...");
  }
}

void Heterotrophs::addChild(std::unique_ptr<Heterotroph> child) {
  children_.push_back(std::move(child));
}

void Heterotrophs::clearChildren() {
  children_.clear();
}

std::vector<std::unique_ptr<Heterotroph>>& Heterotrophs::getHeterotrophs() {
  return heterotrophs_;
}

std::uint32_t Heterotrophs::getLivingCount() const {
  return static_cast<std::uint32_t>(alive_.size());
}


std::uint32_t Heterotrophs::getDeadCount() const {
  return static_cast<std::uint32_t>(dead_.size());
}
