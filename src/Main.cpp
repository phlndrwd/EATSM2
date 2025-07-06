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
#include "Date.h"
#include "Environment.h"
#include "TimeStep.h"

#include "Data.h"
#include "JsonReader.h"

std::int32_t main() {
  std::cout << consts::kSystemName + " " + consts::kSystemVersion + " starting on "
            << Date::getDateAndTimeString() << "..." << std::endl << std::endl;
  jino::Data paramsData;
  jino::JsonReader reader;

  const std::string paramFilePath = consts::kConfigurationDirectory + consts::kParamsFile;
  reader.readParams(paramsData, paramFilePath, consts::getParamNames());

  Parameters params(paramsData);

  const std::uint64_t samplingRate = paramsData.getValue<std::uint64_t>(consts::kParamNames.at(enums::eSamplingRate));
  const std::uint64_t maxTimeStep = paramsData.getValue<std::uint64_t>(consts::kParamNames.at(enums::eMaxTimeStep));

  Environment environment(params);
  TimeStep timeStep(params.getSamplingRate());

  std::cout << "Starting main time loop..." << std::endl;
  for (std::uint64_t t = 0; t < maxTimeStep; ++t) {
    environment.update();
    if (t % samplingRate == 0) {
      // jino::Buffers::get().record();
      // output.writeDatums(data);
      std::cout << "t=" << t << std::endl;
    }
  }

  std::cout << "Complete." << std::endl;

  return 0;
}
