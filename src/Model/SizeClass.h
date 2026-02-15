/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef living_H
#define living_H

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <memory>
#include <stdfloat>
#include <vector>

#include "Heterotroph.h"
#include "Nutrient.h"
#include "Parameters.h"
#include "RandomSimple.h"

namespace {
std::int32_t roundWithProbability(RandomSimple& random, const std::float64_t value) {
  std::int32_t flooredValue = static_cast<std::int32_t>(std::floor(value));
  std::float64_t probability = value - flooredValue;
  if (random.getUniform() < probability) {
    return flooredValue + 1;
  } else {
    return flooredValue;
  }
}
}  // anonymous namespace

class SizeClass {
public:
  SizeClass() = delete;
  SizeClass(const SizeClass&) = delete;
  SizeClass& operator=(const SizeClass&) = delete;

  SizeClass(SizeClass&&) noexcept = default;
  SizeClass& operator=(SizeClass&&) noexcept = default;
  explicit SizeClass(Parameters*, const std::uint32_t);

  void forEachHeterotroph(auto&& func) {
    for (std::uint32_t index = 0; index < getLivingCount(); ++index) {
      func(index, living_[index].get());
    }
  }

  void forEachHeterotroph(auto&& func) const {
    for (const auto& heterotroph : living_) {
      func(heterotroph.get());
    }
  }

  void subset(RandomSimple& random, auto&& func) {
    std::uint32_t populationSize = static_cast<std::uint32_t>(living_.size());
    std::uint32_t subsetCount = roundWithProbability(random, populationSize * subsetFraction_);
    for (auto _ = subsetCount; _--;) {
      Heterotroph* randomHeterotroph = nullptr;
      std::uint32_t randomIndex = 0;
      do {
        randomIndex = random.getUniformInt(populationSize - 1);
        randomHeterotroph = living_[randomIndex].get();
      } while (!randomHeterotroph->isAlive());
      func(randomHeterotroph);
    }
  }

  void forEachChild(auto&& func) {
    for (auto& child : children_) {
      func(std::move(child));
    }
  }

  Heterotroph* keyHeterotroph(const std::uint32_t);
  const Heterotroph* keyHeterotroph(const std::uint32_t) const;

  Heterotroph* getRandomHeterotroph(RandomSimple&);

  std::unique_ptr<Heterotroph> ownHeterotroph(const std::uint32_t);

  void addHeterotroph(std::unique_ptr<Heterotroph>);
  void addChild(std::unique_ptr<Heterotroph>);
  void clearChildren();
  void removeDead();

  std::uint32_t getLivingCount() const;

  const std::uint32_t& getIndex() const;

  const std::float64_t& getSizeClassUpper() const;
  const std::float64_t& getSizeClassMidPoint() const;
  const std::float64_t& getSizeClassLower() const;

private:
  const std::uint32_t index_;
  const std::uint32_t maxPopulation_;

  const std::float64_t sizeClassUpper_;
  const std::float64_t sizeClassMidPoint_;
  const std::float64_t sizeClassLower_;

  const std::float64_t subsetFraction_;

  std::vector<std::unique_ptr<Heterotroph>> living_;
  std::vector<std::unique_ptr<Heterotroph>> children_;
};

#endif // living_H
