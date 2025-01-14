/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Traits.h"

#include "Parameters.h"
#include "RandomSimple.h"

Traits::Traits(const std::vector<double>& values, const std::vector<std::uint8_t>& areMutantTraits,
               const double& mutationProbability_, const double& mutationStandardDeviation_):
    mutationProbability_(mutationProbability_),
    mutationStandardDeviation_(mutationStandardDeviation_) {
  values_.insert(values_.end(), values.begin(), values.end());
  areMutantTraits_.insert(areMutantTraits_.end(), areMutantTraits.begin(), areMutantTraits.end());
}

Traits::Traits(const Traits& traits) {
  if (this != &traits) {
    mutationProbability_ = traits.mutationProbability_;
    mutationStandardDeviation_ = traits.mutationStandardDeviation_;
    values_ = traits.values_;
    areMutantTraits_ = traits.areMutantTraits_;
  }
}

Traits::Traits(Traits&& traits) noexcept {
  if (this != &traits) {
    mutationProbability_ = std::move(traits.mutationProbability_);
    mutationStandardDeviation_ = std::move(traits.mutationStandardDeviation_);
    values_ = std::move(traits.values_);
    areMutantTraits_ = std::move(traits.areMutantTraits_);
  }
}

Traits& Traits::operator=(const Traits& traits) {
  if (this != &traits) {
    mutationProbability_ = traits.mutationProbability_;
    mutationStandardDeviation_ = traits.mutationStandardDeviation_;
    values_ = traits.values_;
    areMutantTraits_ = traits.areMutantTraits_;
  }
  return *this;
}

Traits& Traits::operator=(Traits&& traits) noexcept {
  if (this != &traits) {
    values_ = std::move(traits.values_);
    areMutantTraits_ = std::move(traits.areMutantTraits_);
  }
  return *this;
}

const Traits Traits::getChildTraits(RandomSimple& random) {
  std::size_t numberOfGenes = values_.size();
  std::vector<double> childValues = values_;
  std::vector<std::uint8_t> areTraitsMutations(numberOfGenes, 0);

  if (mutationProbability_ > 0) {
    for (std::size_t i = 0; i < numberOfGenes; ++i) {
      if (random.getUniform() <= mutationProbability_) {
        areTraitsMutations[i] = true;  // PJU FIX - What gets assigned here?

        double mutationValue = random.getNormal(0.0, mutationStandardDeviation_);

        childValues[i] += mutationValue;

        // Perform reflection on mutations outside the required range 0 to 1.
        if (childValues[i] < 0) {
          childValues[i] = 0 - childValues[i];
        } else if (childValues[i] > 1) {
          childValues[i] = 2 - childValues[i];
        }
      }
    }
  }
  return Traits(childValues, areTraitsMutations, mutationProbability_, mutationStandardDeviation_);
}

const std::vector<std::uint8_t>& Traits::areTraitsMutant() const {
  return areMutantTraits_;
}

const std::vector<double>& Traits::getValues() const {
  return values_;
}

bool Traits::isTraitMutant(const std::uint32_t traitIndex) const {
  return areMutantTraits_[traitIndex] != 0;
}

const double& Traits::getValue(const enums::eTraitIndices trait) const {
  return values_[trait];
}

void Traits::setValue(const enums::eTraitIndices trait, const double traitValue) {
  values_[trait] = traitValue;
}
