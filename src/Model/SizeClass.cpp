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
  heterotrophs_.reserve(maxPopulation_);
}

Heterotroph& SizeClass::keyHeterotroph(const std::uint32_t index) {
  assert(index < heterotrophs_.size());
  assert(heterotrophs_[index] != nullptr);
  return *heterotrophs_[index];
}

const Heterotroph& SizeClass::keyHeterotroph(const std::uint32_t index) const {
  assert(index < heterotrophs_.size());
  assert(heterotrophs_[index] != nullptr);
  return *heterotrophs_[index];
}

std::unique_ptr<Heterotroph> SizeClass::ownHeterotroph(const std::uint32_t index) {
  assert(index < heterotrophs_.size());
  assert(heterotrophs_[index] != nullptr);
  return std::move(heterotrophs_[index]);
}

void SizeClass::removeHeterotroph(const std::uint32_t index) {
  assert(index < heterotrophs_.size());
  heterotrophs_.erase(std::next(std::begin(heterotrophs_), index));
}

void SizeClass::addHeterotroph(std::unique_ptr<Heterotroph> heterotroph) {
  assert(heterotrophs_.size() < maxPopulation_);
  heterotrophs_.push_back(std::move(heterotroph));
}

void SizeClass::addChild(std::unique_ptr<Heterotroph> child) {
  children_.push_back(std::move(child));
}

void SizeClass::clearChildren() {
  children_.clear();
}

std::uint32_t SizeClass::getSize() const {
  return static_cast<std::uint32_t>(heterotrophs_.size());
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

