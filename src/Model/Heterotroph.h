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

#include "Traits.h"

class EcologicalFunctions;
class RandomSimple;

class Heterotroph {
 public:
  Heterotroph() = delete;
  explicit Heterotroph(std::vector<double>&, std::vector<unsigned char>&,
                       const double, const double, const double, const double);
  explicit Heterotroph(const Traits&, const double, const double, const double, const double, const double);

  Heterotroph(const Heterotroph&);
  Heterotroph(const Heterotroph&&) noexcept;

  Heterotroph& operator=(const Heterotroph&);
  Heterotroph& operator=(const Heterotroph&&);

  Heterotroph* getChild(RandomSimple&, const EcologicalFunctions&);

  double consumePreyVolume(const double);
  double metabolise(const double);

  Traits& getHeritableTraits();
  double getTrophicLevel() const;
  unsigned getAge() const;

  bool hasFed() const;
  bool isDead() const;

  double getVolumeActual() const;
  double getVolumeHeritable() const;
  double getVolumeMinimum() const;
  double getVolumeReproduction() const;

  double getStarvationMultiplier() const;

  void setTrophicLevel(const double);

  void setAge(const unsigned);
  void setHasFed(const bool);
  void setDead();

 private:
  Traits traits_;
  double volumeHeritable_;

  double volumeActual_;
  double volumeMinimum_;
  double volumeReproduction_;

  double assimilationEfficiency_;
  double starvationMultiplier_;

  double trophicLevel_;
  unsigned age_;
  bool hasFed_;
  bool isDead_;
};

#endif
