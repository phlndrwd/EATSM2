/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Life.h"

#include <algorithm>
#include <climits>

#include "Constants.h"
#include "Parameters.h"

Life::Life(Nutrient& nutrient, Parameters& params) :
    nutrient_(nutrient),
    params_(params),
    autotrophs_(nutrient, params.getInitialAutotrophVolume(), params.getDataSize()),
    heterotrophs_(&nutrient, &autotrophs_, &params, params.getRandomSeed()) // Is this the first time random is used?
 {}

void Life::update() {
  autotrophs_.update();
  heterotrophs_.update();
}

