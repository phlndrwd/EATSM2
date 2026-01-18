/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Traits.h"

#include <cassert>

#include "Parameters.h"
#include "RandomSimple.h"

Traits::Traits(const TraitValues& values, const std::float64_t& mutationProbability_, const std::float64_t& mutationStandardDeviation_) :
    values_(values),
    mutationProbability_(mutationProbability_),
    mutationStandardDeviation_(mutationStandardDeviation_) {}

Traits::Traits(const Traits& traits) :
    values_(traits.values_),
    mutationProbability_(mutationProbability_),
    mutationStandardDeviation_(mutationStandardDeviation_) {
  assert(this != &traits);
}

Traits::Traits(Traits&& traits) noexcept  :
    values_(traits.values_),
    mutationProbability_(mutationProbability_),
    mutationStandardDeviation_(mutationStandardDeviation_) {
  assert(this != &traits);
}

Traits& Traits::operator=(const Traits& traits) {
  if (this != &traits) {
    mutationProbability_ = traits.mutationProbability_;
    mutationStandardDeviation_ = traits.mutationStandardDeviation_;
    values_ = traits.values_;
  }
  return *this;
}

Traits& Traits::operator=(Traits&& traits) noexcept {
  if (this != &traits) {
    values_ = std::move(traits.values_);
  }
  return *this;
}

const Traits Traits::getChildTraits(RandomSimple& random) {
  TraitValues childValues(values_);
  if (mutationProbability_ > 0) {
    if (random.getUniform() <= mutationProbability_) {
      childValues.volumeIsMutant = true;
      childValues.volume += random.getNormal(0.0, mutationStandardDeviation_);

      // Perform reflection on mutations outside the required range 0 to 1.
      std::float64_t volValue = childValues.volume;
      if (volValue < 0) {
        childValues.volume = 0 - volValue;
      } else if (volValue > 1) {
        childValues.volume = 2 - volValue;
      }
    }
  }
  return Traits(childValues, mutationProbability_, mutationStandardDeviation_);
}

const TraitValues& Traits::getValues() const {
  return values_;
}

void Traits::setValues(const TraitValues& values) {
  values_ = values;
}

