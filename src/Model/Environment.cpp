/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Environment.h"

#include<iostream>

Environment::Environment(): nutrient_(), life_(nutrient_) {
  std::cout << "Environment created." << std::endl << std::endl;
}

void Environment::update() {
  life_.update();
}

bool Environment::recordData() {
  //bool isAlive = population_.recordData();
  nutrient_.recordData();

  return true;
}

Nutrient& Environment::getNutrient() {
  return nutrient_;
}

const Nutrient& Environment::getNutrient() const {
  return nutrient_;
}
