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
#include <vector>

#include "Enums.h"

class RandomSimple;

class Traits {
 public:
  Traits() = delete;
  explicit Traits(const std::vector<double>&, const std::vector<std::uint8_t>&, const double&, const double&);

  Traits(const Traits&);
  Traits(Traits&&) noexcept;

  Traits& operator=(const Traits&);
  Traits& operator=(Traits&&) noexcept;

  const Traits getChildTraits(RandomSimple&);
  const std::vector<double>& getValues() const;
  const std::vector<std::uint8_t>& areTraitsMutant() const;

  const double& getValue(const enums::eTraitIndices) const;
  bool isTraitMutant(const std::uint32_t) const;

  void setValue(const enums::eTraitIndices, const double);

 private:
  double mutationProbability_;
  double mutationStandardDeviation_;

  std::vector<double> values_;
  std::vector<std::uint8_t> areMutantTraits_;
};

#endif
