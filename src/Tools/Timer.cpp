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

Timer::Timer(const std::uint32_t& runTimeInSeconds, bool startNow) : runTimeInSeconds_(runTimeInSeconds) {
  if (startNow == true) {
    start();
  }
}

Timer::~Timer() {}

void Timer::start() {
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
  // PJU FIX - Under certain conditions (debugging) this function can output an impossibly high number.
  //            Changing to ints does not fix this.
  std::uint32_t secondsRemaining = runTimeInSeconds_ - elapsed();
  std::uint32_t minutesRemaining = secondsRemaining / consts::kSecondsInAMinute;
  secondsRemaining = secondsRemaining - (minutesRemaining * consts::kSecondsInAMinute);
  std::uint32_t hoursRemaining = minutesRemaining / consts::kMinutesInAnHour;
  minutesRemaining = minutesRemaining - (hoursRemaining * consts::kMinutesInAnHour);
  std::uint32_t daysRemaining = hoursRemaining / consts::kHoursInADay;
  hoursRemaining = hoursRemaining - (daysRemaining * consts::kHoursInADay);

  std::string str = "";
  if (daysRemaining > 0) str.append(std::to_string(daysRemaining)).append("d");
  str.append(std::to_string(hoursRemaining)).append("h");
  str.append(std::to_string(minutesRemaining)).append("m");
  str.append(std::to_string(secondsRemaining)).append("s");

  return str;
}
