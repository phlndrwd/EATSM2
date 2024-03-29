/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Date.h"

#include <ctime>
#include <sys/time.h>  // For struct timeval

#include "Constants.h"

std::string Date::getDateAndTimeString(const std::string format, unsigned addedSeconds) {
  char dateTimeChar[constants::kDateTimeBufferSize];
  timeval timeNow;
  gettimeofday(&timeNow, nullptr);
  time_t rawtime = static_cast<time_t>(timeNow.tv_sec + addedSeconds);

  struct tm *timeinfo;
  timeinfo = localtime(&rawtime);

  std::strftime(dateTimeChar, constants::kDateTimeBufferSize, format.c_str(), timeinfo);
  std::string dateTime(dateTimeChar);

  return dateTime;
}
