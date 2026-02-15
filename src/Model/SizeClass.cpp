/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "SizeClass.h"

#include <cassert>

namespace {



}  // anonymous namespace

SizeClass::SizeClass(Parameters* params, const std::uint32_t index) :
        index_(index),
        maxPopulation_(params->getMaximumSizeClassPopulation(index)),
        sizeClassUpper_(params->getSizeClassBoundary(index + 1)),
        sizeClassMidPoint_(params->getSizeClassMidPoint(index)),
        sizeClassLower_(params->getSizeClassBoundary(index)),
        subsetFraction_(params->getSizeClassSubsetFraction()) {
  living_.reserve(maxPopulation_);
}

Heterotroph* SizeClass::keyHeterotroph(const std::uint32_t index) {
  assert(index < living_.size());
  assert(living_[index] != nullptr);
  return living_[index].get();
}

const Heterotroph* SizeClass::keyHeterotroph(const std::uint32_t index) const {
  assert(index < living_.size());
  assert(living_[index] != nullptr);
  return living_[index].get();
}

Heterotroph* SizeClass::getRandomHeterotroph(RandomSimple& random) {
  std::uint32_t randomIndex = random.getUniformInt(getLivingCount() - 1);
  assert(randomIndex < living_.size());
  assert(living_[randomIndex] != nullptr);
  return living_[randomIndex].get();
}

std::unique_ptr<Heterotroph> SizeClass::ownHeterotroph(const std::uint32_t index) {
  assert(index < living_.size());
  assert(living_[index] != nullptr);
  return std::move(living_[index]);
}

void SizeClass::removeDead() {
  std::erase_if(living_, [](const std::unique_ptr<Heterotroph>& heterotroph) {
    return !heterotroph || !heterotroph->isAlive();
  });
}

void SizeClass::addHeterotroph(std::unique_ptr<Heterotroph> heterotroph) {
  assert(living_.size() < maxPopulation_);
  living_.push_back(std::move(heterotroph));
}

void SizeClass::addChild(std::unique_ptr<Heterotroph> child) {
  children_.push_back(std::move(child));
}

void SizeClass::clearChildren() {
  children_.clear();
}

std::uint32_t SizeClass::getLivingCount() const {
  return static_cast<std::uint32_t>(living_.size());
}

const std::uint32_t& SizeClass::getIndex() const {
  return index_;
}

const std::float64_t& SizeClass::getSizeClassUpper() const {
  return sizeClassUpper_;
}

const std::float64_t& SizeClass::getSizeClassMidPoint() const {
  return sizeClassMidPoint_;
}

const std::float64_t& SizeClass::getSizeClassLower() const {
  return sizeClassLower_;
}

