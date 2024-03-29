/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef AUTOTROPHS
#define AUTOTROPHS

class Nutrient;

class Autotrophs {
 public:
  Autotrophs() = delete;
  explicit Autotrophs(Nutrient&, const double);

  void update();
  void addToVolume(const double);
  void subtractFromVolume(const double);
  void recordData();

  double& getVolume();
  const double& getVolume() const;

 private:
  Nutrient& nutrient_;

  double volume_;
  double toFlux_;
};

#endif
