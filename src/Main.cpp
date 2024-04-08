/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include <cstdint>
#include <iostream>

#include "Constants.h"
#include "DataRecorder.h"
#include "Date.h"
#include "Environment.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "Parameters.h"
#include "TimeStep.h"

#include "Timer.h"

std::int32_t main() {
  std::cout << consts::kSystemName + " " + consts::kSystemVersion + " starting on "
            << Date::getDateAndTimeString() << "..." << std::endl
            << std::endl;
  Parameters params;
  FileReader fileReader;
  fileReader.setParameters(params);
  Timer timer(params.getRunTimeInSeconds(), true);
  FileWriter fileWriter;  // Created here to initialise output directory

  std::uint32_t runTimeInSeconds = params.getRunTimeInSeconds();
  double oneTenthOfRunTimeInSeconds = runTimeInSeconds / 10.0;
  double cumulativeTenthsOfRunTime = 0;
  bool isAlive = true;

  Environment environment(params);
  TimeStep timeStep(params.getSamplingRate());

  std::cout << "Model run due to complete on "
            << Date::getDateAndTimeString(consts::kCompleteDateFormat, runTimeInSeconds) << std::endl
            << std::endl;
  std::cout << "Starting main time loop..." << std::endl;
  do {
    // Update before data collection; calculates essential variables for
    // encounter rates.
    environment.update();
    // Data collection
    if (timeStep.doRecordData() == true) {
      DataRecorder::get()->addDataTo("AxisTimeSteps", timeStep.getTimeStep());
      DataRecorder::get()->addDataTo("TimeSampling", timer.split());
      isAlive = environment.recordData();
    }
    // Text output at the completion of each ten percent of the run
    if (timer.elapsed() >= (std::uint32_t)cumulativeTenthsOfRunTime) {
      cumulativeTenthsOfRunTime = cumulativeTenthsOfRunTime + oneTenthOfRunTimeInSeconds;
      std::cout << "t = " << timeStep.getTimeStep() << consts::kDataDelimiterValue
                << consts::kWhiteSpaceCharacter << timer.remainingString() << " remaining at "
                << Date::getDateAndTimeString() << "..." << std::endl;
    }
    timeStep.incrementTimeStep();
    //std::cout << "timer.Elapsed( )> " << timer.Elapsed( ) << ",
    // runTimeInSeconds> " << runTimeInSeconds << ", isAlive> " << isAlive <<
    // std::endl;
  } while (timer.elapsed() < runTimeInSeconds && isAlive == true);
  std::cout << "Total run time " << timer.stop() << "s" << std::endl;
  return 0;
}
