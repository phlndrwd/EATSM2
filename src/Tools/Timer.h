/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef TIMER
#define TIMER

#include <chrono>
#include <string>

class Timer {
 public:
  explicit Timer(bool goNow = true);
  ~Timer();

  void go();
  double split();
  double elapsed();
  double stop();

  std::string remainingString();

 private:
  const unsigned runTimeInSeconds_;

  std::chrono::high_resolution_clock::time_point startTime_;
  std::chrono::high_resolution_clock::time_point splitTime_;
  std::chrono::high_resolution_clock::time_point elapsedTime_;
  std::chrono::high_resolution_clock::time_point stopTime_;
};

#endif
