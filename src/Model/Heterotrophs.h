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
  explicit Heterotrophs(Nutrient&, Parameters&, const std::float64_t&, const std::uint32_t&, const std::uint32_t&);

  void subset(std::function<void(std::uint32_t)>);

  template <typename F>
  void forEachHeterotrophIndex(F&& func) {
    std::for_each(std::begin(alive_), std::end(alive_), [&](std::uint32_t index) {
      func(index);
    });
  }

  template <typename F>
  void subset(F&& func) {
    std::size_t numberAlive = alive_.size();
    if (numberAlive != 0) {
      std::uint32_t sizeClassSubset = roundWithProbability(random_, numberAlive * subsetFraction_);
      for (auto _ = sizeClassSubset; _--;) {
        func(getRandomHeterotrophIndex());
      }
    }
  }

  template <typename F>
  void forEachChild(F&& func) {
    std::for_each(std::begin(children_), std::end(children_), [&](std::shared_ptr<Heterotroph> child) {
      func(child);
    });
  }

  std::uint32_t getRandomHeterotrophIndex();
  Heterotroph* getRandomHeterotroph();
  Heterotroph* getRandomHeterotroph(std::uint32_t&);
  Heterotroph* getHeterotroph(const std::uint32_t);
  const Heterotroph* getHeterotroph(const std::uint32_t) const;
  std::shared_ptr<Heterotroph> ownHeterotroph(const std::uint32_t);
  void removeHeterotroph(const std::uint32_t);

  void addHeterotroph(std::shared_ptr<Heterotroph>);
  void addChild(std::shared_ptr<Heterotroph>);
  void clearChildren();

  std::vector<std::shared_ptr<Heterotroph>>& getHeterotrophs();

  std::uint64_t getLivingCount();
  std::uint64_t getDeadCount();

private:
  Nutrient& nutrient_;

  RandomSimple random_;

  const std::float64_t sizeClassMidPoint_;
  const std::float64_t subsetFraction_;
  const std::uint32_t maxPopulation_;

  std::vector<std::shared_ptr<Heterotroph>> heterotrophs_;
  std::vector<std::shared_ptr<Heterotroph>> children_;
  std::vector<std::uint32_t> alive_;
  std::queue<std::uint32_t> dead_;
};

#endif // HETEROTROPHS_H
