/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#ifndef CONSTANTS
#define CONSTANTS

#include <cstdint>
#include <string>

namespace consts {
const std::string kConfigurationDirectory = "./input/";
const std::string kOutputDirectoryName = "output";

const std::string kInputParametersFileName = "Parameters.csv";
const std::string kOutputParametersFileName = "Variables.csv";
const std::string kInitialStateFileName = "InitialState.csv";
const std::string kModelStateFileName = "State.csv";
const std::string kFileNameExtension = ".csv";

const std::string kVectorDatumTypeName = "vector";
const std::string kMatrixDatumTypeName = "matrix";

const std::uint32_t kMaximumNumberOfTrophicLevels = 11;  // 0 = unclassified, 1 = primary, etc.
const int kMissingValue = -9999;

const std::uint32_t kDateTimeBufferSize = 25;
const std::uint32_t kOutputFolderPermissions = 0777;

const std::uint32_t kAutotrophSizeIndex = 0;
const std::uint32_t kReproductionFactor = 2;
const double kReproductionMultiplier = 1 / (double)kReproductionFactor;  // Equivalent to 1 / 2
const double kMinimumFractionalVolume = 0.5;

const std::uint32_t kSecondsInAMinute = 60;
const std::uint32_t kMinutesInAnHour = 60;
const std::uint32_t kHoursInADay = 24;

const std::string kCompleteDateFormat = "%c";
const std::string kDataSetNameFormat = "%Y-%m-%d_%H-%M-%S";

const char kFolderDelimiter = '/';
const char kWhiteSpaceCharacter = ' ';
const char kDataDelimiterValue = ',';
const char kCommentCharacter = '#';

const std::string kParameterFileCommand = "-p";
const std::string kStateFileCommand = "-s";
const std::string kRestartCommand = "-r";
const std::string kVersionCommand = "-v";

const std::string kSystemName = "EATSM2";
const std::string kSystemVersion = "0.0.1";
const std::string kSystemDate = "29/01/2023";
const std::string kSystemTime = "00:00";
}  // namespace Constants

#endif
