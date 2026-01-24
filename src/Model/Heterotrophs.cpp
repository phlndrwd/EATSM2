/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Heterotrophs.h"

#include <cassert>

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
  assert(!alive_.empty());
  assert(index < heterotrophs_.size());
  assert(heterotrophs_[index] != nullptr);
  return *heterotrophs_[index];
}

const Heterotroph& Heterotrophs::keyHeterotroph(const std::uint32_t index) const {
  assert(!alive_.empty());
  assert(index < heterotrophs_.size());
  assert(heterotrophs_[index] != nullptr);
  return *heterotrophs_[index];
}

std::unique_ptr<Heterotroph> Heterotrophs::ownHeterotroph(const std::uint32_t index) {
  assert(!alive_.empty());
  assert(index < heterotrophs_.size());
  assert(heterotrophs_[index] != nullptr);
  return std::move(heterotrophs_[index]);
}

void Heterotrophs::removeHeterotroph(const std::uint32_t index) {
  assert(!alive_.empty());

  auto it = std::find(alive_.begin(), alive_.end(), index);
  assert(it != alive_.end());

  alive_.erase(it);
  heterotrophs_[index].reset();
  dead_.push(index);
}

void Heterotrophs::addHeterotroph(std::unique_ptr<Heterotroph> heterotroph) {
  assert(alive_.size() != maxPopulation_);

  std::int32_t index;
  if (!dead_.empty()) {
    index = dead_.front();
    assert(!heterotrophs_[index]);  // Slot must be empty
    dead_.pop();
  } else {
    index = heterotrophs_.size();
    heterotrophs_.emplace_back(nullptr);
  }
  heterotrophs_[index] = std::move(heterotroph);
  alive_.push_back(index);
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

std::uint32_t Heterotrophs::getLivingIndex(const std::uint32_t index) {
  assert(!alive_.empty());
  assert(index < alive_.size());
  return alive_[index];
}
