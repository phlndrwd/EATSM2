/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef DATE
#define DATE

#include "Constants.h"

class Date {
 public:
  static std::string getDateAndTimeString(const std::string format = consts::kCompleteDateFormat,
                                          std::uint32_t addedSeconds = 0);
};

#endif
