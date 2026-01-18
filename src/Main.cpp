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
#include "Environment.h"
#include "TimeStep.h"

#include "Buffers.h"
#include "Data.h"
#include "JsonReader.h"
#include "Output.h"

std::int32_t main() {
  jino::Data attrs;
  jino::Data paramsData;
  jino::JsonReader reader;
  jino::Output output;
  jino::NetCDFData data;

  std::cout << consts::kSystemName + " " + consts::kSystemVersion + " starting on "
            << output.getDate() << "..." << std::endl << std::endl;

  const std::string attrsPath = consts::kConfigurationDirectory + consts::kAttrsFile;
  const std::string paramFilePath = consts::kConfigurationDirectory + consts::kParamsFile;

  reader.readParams(paramsData, paramFilePath, consts::getParamNames());
  reader.readAttrs(attrs, attrsPath);

  data.addDateToData(&attrs, output.getDate());
  data.addData(&paramsData);

  Parameters params(paramsData);
  Environment environment(params);
  TimeStep timeStep(params.getSamplingRate());


  const std::uint64_t samplingRate = params.getSamplingRate();
  const std::uint64_t maxTimeStep = params.getMaxTimeStep();

  data.addDimension("dataSize", params.getDataSize(), true);
  data.addDimension("numberOfSizeClasses", params.getNumberOfSizeClasses(), false);

  output.writeMetadata(data);
  std::cout << "Starting main time loop..." << std::endl;
  for (std::uint64_t t = 0; t < maxTimeStep; ++t) {
    environment.update();
    if (t % samplingRate == 0) {
      jino::Buffers::get().record();
      output.writeDatums(data);
      std::cout << "t=" << t << std::endl;
    }
  }
  output.closeNetCDF();
  output.waitForCompletion();
  std::cout << "Complete." << std::endl;

  return 0;
}
