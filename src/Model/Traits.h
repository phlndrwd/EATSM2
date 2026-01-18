/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef TRAITS_H
#define TRAITS_H

#include <cstdint>
#include <stdfloat>
#include <vector>

class RandomSimple;

struct TraitValues {
  std::float64_t volume;
  std::uint8_t volumeIsMutant;
  TraitValues(std::float64_t _volume) : volume(_volume), volumeIsMutant(false) {}
  TraitValues(std::float64_t _volume, std::uint8_t _volumeIsMutant) : volume(_volume), volumeIsMutant(_volumeIsMutant) {}
};

class Traits {
 public:
  Traits() = delete;
  explicit Traits(const TraitValues&, const std::float64_t&, const std::float64_t&);

  Traits(const Traits&);
  Traits(Traits&&) noexcept;

  Traits& operator=(const Traits&);
  Traits& operator=(Traits&&) noexcept;

  const Traits getChildTraits(RandomSimple&);

  const TraitValues& getValues() const;
  void setValues(const TraitValues&);

 private:
  std::float64_t mutationProbability_;
  std::float64_t mutationStandardDeviation_;

  TraitValues values_;
};

#endif
