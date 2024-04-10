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

#include <cstdint>
#include <functional>
#include <queue>
#include <vector>

#include "Heterotroph.h"
#include "Nutrient.h"
#include "Parameters.h"
#include "RandomSimple.h"

class Heterotrophs {
public:
  Heterotrophs() = delete;
  explicit Heterotrophs(Nutrient&, Parameters&, EcologicalFunctions&, const double&,
                        const std::uint32_t&, const std::uint32_t&);

  void subset(std::function<void(std::uint32_t)>);

  template <typename F>
  void forEachHeterotrophIndex(F&& func) {
    std::for_each(std::begin(alive_), std::end(alive_), [&](std::uint32_t index) {
      func(index);
    });
  }

  template <typename F>
  void forEachChild(F&& func) {
    std::for_each(std::begin(children_), std::end(children_), [&](Heterotroph child) {
      func(child);
    });
  }

  std::size_t getPopulationSize();
  std::uint32_t getRandomHeterotrophIndex();
  Heterotroph& getRandomHeterotroph();
  Heterotroph& getRandomHeterotroph(std::uint32_t&);
  Heterotroph& getHeterotroph(const std::uint32_t);
  const Heterotroph& getHeterotroph(const std::uint32_t) const;
  void removeHeterotroph(const std::uint32_t);

  void addHeterotroph(Heterotroph);
  void addChild(Heterotroph);
  void clearChildren();

  std::vector<Heterotroph>& getHeterotrophs();

private:
  Nutrient& nutrient_;
  EcologicalFunctions& functions_;

  RandomSimple random_;

  const double sizeClassMidPoint_;
  const double subsetFraction_;
  const std::uint32_t maxPopulation_;

  std::vector<Heterotroph> heterotrophs_;
  std::vector<Heterotroph> children_;
  std::vector<std::uint32_t> alive_;
  std::queue<std::uint32_t> dead_;
};

#endif // HETEROTROPHS_H
