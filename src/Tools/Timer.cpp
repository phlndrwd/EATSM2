/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Timer.h"

#include "Constants.h"
#include "Parameters.h"

Timer::Timer(bool goNow) : runTimeInSeconds_(Parameters::Get()->getRunTimeInSeconds()) {
  if (goNow == true) go();
}

Timer::~Timer() {}

void Timer::go() {
  startTime_ = std::chrono::high_resolution_clock::now();
  splitTime_ = startTime_;
}

double Timer::elapsed() {
  elapsedTime_ = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed;

  elapsed = elapsedTime_ - startTime_;

  return elapsed.count();
}

double Timer::split() {
  std::chrono::high_resolution_clock::time_point timeNow = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> split;

  split = timeNow - splitTime_;
  splitTime_ = timeNow;

  return split.count();
}

double Timer::stop() {
  stopTime_ = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> total;

  total = stopTime_ - startTime_;

  return total.count();
}

std::string Timer::remainingString() {
  unsigned secondsRemaining = runTimeInSeconds_ - elapsed();
  unsigned minutesRemaining = secondsRemaining / constants::kSecondsInAMinute;
  secondsRemaining = secondsRemaining - (minutesRemaining * constants::kSecondsInAMinute);
  unsigned hoursRemaining = minutesRemaining / constants::kMinutesInAnHour;
  minutesRemaining = minutesRemaining - (hoursRemaining * constants::kMinutesInAnHour);
  unsigned daysRemaining = hoursRemaining / constants::kHoursInADay;
  hoursRemaining = hoursRemaining - (daysRemaining * constants::kHoursInADay);

  std::string str = "";
  if (daysRemaining > 0) str.append(std::to_string(daysRemaining)).append("d");
  str.append(std::to_string(hoursRemaining)).append("h");
  str.append(std::to_string(minutesRemaining)).append("m");
  str.append(std::to_string(secondsRemaining)).append("s");

  return str;
}
