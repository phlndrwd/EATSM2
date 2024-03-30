/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef NUTRIENT_H
#define NUTRIENT_H

class Nutrient {
 public:
  Nutrient();

  void recordData();

  const double& getVolume() const;

  void addToVolume(const double);
  void subtractFromVolume(const double);

 private:
  double volume_;
  double toFlux_;
};

#endif
