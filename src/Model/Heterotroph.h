/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef HETEROTROPH_H
#define HETEROTROPH_H

#include <memory>
#include <stdfloat>

#include "Traits.h"

class RandomSimple;

class Heterotroph {
 public:
  Heterotroph() = delete;

  explicit Heterotroph(const std::float64_t&, const std::float64_t&, const std::float64_t&, const std::float64_t&, const std::float64_t&);
  explicit Heterotroph(const Traits&, const std::float64_t&, const std::float64_t&, const std::float64_t&, const std::float64_t&);

  Heterotroph(const Heterotroph&);
  Heterotroph(const Heterotroph&&) noexcept;

  Heterotroph& operator=(const Heterotroph&);
  Heterotroph& operator=(const Heterotroph&&);

  std::unique_ptr<Heterotroph> getChild(RandomSimple&, const std::float64_t&, const std::float64_t&);

  std::float64_t consumePreyVolume(const std::float64_t);
  std::float64_t metabolise(const std::float64_t);

  Traits& getHeritableTraits();

  std::float64_t getVolumeActual() const;
  std::float64_t getVolumeHeritable() const;
  std::float64_t getVolumeMinimum() const;
  std::float64_t getVolumeReproduction() const;

  std::float64_t getStarvationMultiplier() const;

 private:
  Traits traits_;
  std::float64_t volumeHeritable_;

  std::float64_t volumeActual_;
  std::float64_t volumeMinimum_;
  std::float64_t volumeReproduction_;

  std::float64_t assimilationEfficiency_;
  std::float64_t starvationMultiplier_;
};

#endif
