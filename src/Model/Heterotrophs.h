/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef HETEROTROPHS_H
#define HETEROTROPHS_H

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <memory>
#include <queue>
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

class Heterotrophs {
public:
  Heterotrophs() = delete;
  Heterotrophs(const Heterotrophs&) = delete;
  Heterotrophs& operator=(const Heterotrophs&) = delete;

  Heterotrophs(Heterotrophs&&) noexcept = default;
  Heterotrophs& operator=(Heterotrophs&&) noexcept = default;
  explicit Heterotrophs(Nutrient&, Parameters&, const std::uint32_t);

  void subset(std::function<void(std::uint32_t)>);

  template <typename F>
  void forEachHeterotrophIndex(F&& func) {
    std::for_each(std::begin(alive_), std::end(alive_), [&](const std::uint32_t index) {
      func(index);
    });
  }

  template <typename F>
  void subset(RandomSimple& random, F&& func) {
    const std::uint32_t numberAlive = getLivingCount();
    if (numberAlive != 0) {
      std::uint32_t sizeClassSubset = roundWithProbability(random, numberAlive * subsetFraction_);
      for (auto _ = sizeClassSubset; _--;) {
        const std::uint32_t randomIndex = random.getUniformInt(0, numberAlive - 1);
        func(randomIndex);
      }
    }
  }

  template <typename F>
  void forEachChild(F&& func) {
    for (auto& child : children_) {
      func(std::move(child));
    }
  }

  Heterotroph& keyHeterotroph(const std::uint32_t);
  const Heterotroph& keyHeterotroph(const std::uint32_t) const;

  std::unique_ptr<Heterotroph> ownHeterotroph(const std::uint32_t);
  void removeHeterotroph(const std::uint32_t);

  void addHeterotroph(std::unique_ptr<Heterotroph>);
  void addChild(std::unique_ptr<Heterotroph>);
  void clearChildren();

  std::vector<std::unique_ptr<Heterotroph>>& getHeterotrophs();

  std::uint32_t getLivingCount() const;
  std::uint32_t getDeadCount() const;

private:
  Nutrient& nutrient_;

  const std::float64_t subsetFraction_;
  const std::uint32_t maxPopulation_;

  std::vector<std::unique_ptr<Heterotroph>> heterotrophs_;
  std::vector<std::unique_ptr<Heterotroph>> children_;
  std::vector<std::uint32_t> alive_;
  std::queue<std::uint32_t> dead_;
};

#endif // HETEROTROPHS_H
