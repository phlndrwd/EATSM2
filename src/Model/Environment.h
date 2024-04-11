/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Life.h"
#include "Nutrient.h"
#include "Parameters.h"

class Environment {
 public:
  Environment(Parameters&);

  void update();
  void snapshot();

  Nutrient& getNutrient();
  const Nutrient& getNutrient() const;

 private:
  Nutrient nutrient_;
  Life life_;
};

#endif
