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

#include <array>
#include <cstdint>
#include <stdfloat>
#include <string>
#include <vector>

#include "Parameters.h"

namespace consts {
constexpr std::string kConfigurationDirectory = "./input/";
constexpr std::string kParamsFile = "params.json";
constexpr std::string kAttrsFile = "attrs.json";

constexpr std::string kOutputDirectoryName = "output";

constexpr std::string kInputParametersFileName = "Parameters.csv";
constexpr std::string kOutputParametersFileName = "Variables.csv";
constexpr std::string kModelStateFileName = "State.csv";
constexpr std::string kFileNameExtension = ".csv";

constexpr std::string kVectorDatumTypeName = "vector";
constexpr std::string kMatrixDatumTypeName = "matrix";

constexpr std::uint32_t kMaximumNumberOfTrophicLevels = 11;  // 0 = unclassified, 1 = primary, etc.
constexpr std::int32_t kMissingValue = -9999;

constexpr std::uint32_t kDateTimeBufferSize = 25;
constexpr std::uint32_t kOutputFolderPermissions = 0777;

constexpr std::uint32_t kAutotrophSizeIndex = 0;
constexpr std::uint32_t kReproductionFactor = 2;
constexpr std::float64_t kReproductionMultiplier = 1 / (std::float64_t)kReproductionFactor;  // Equivalent to 1 / 2
constexpr std::float64_t kMinimumFractionalVolume = 0.5;

constexpr std::uint32_t kSecondsInAMinute = 60;
constexpr std::uint32_t kMinutesInAnHour = 60;
constexpr std::uint32_t kHoursInADay = 24;

constexpr std::string kCompleteDateFormat = "%c";
const std::string kDataSetNameFormat = "%Y-%m-%d_%H-%M-%S";

constexpr char kFolderDelimiter = '/';
constexpr char kWhiteSpaceCharacter = ' ';
constexpr char kDataDelimiterValue = ',';
constexpr char kCommentCharacter = '#';

constexpr std::string kParameterFileCommand = "-p";
constexpr std::string kStateFileCommand = "-s";
constexpr std::string kRestartCommand = "-r";
constexpr std::string kVersionCommand = "-v";

constexpr std::string kSystemName = "EATSM2";
constexpr std::string kSystemVersion = "0.0.1";
constexpr std::string kSystemDate = "29/01/2023";
constexpr std::string kSystemTime = "00:00";

const std::array<std::string, eNumberOfParamters> kParamNames = {
  "RandomSeed",
  "MaxTimeStep",
  "SamplingRate",
  "NumberOfSizeClasses",
  "ReadModelState",
  "WriteModelState",
  "UseLinearFeeding",
  "InitialAutotrophicVolume",
  "InitialHeterotrophicVolume",
  "MinimumHeterotrophicVolume",
  "SmallestIndividualVolume",
  "LargestIndividualVolume",
  "SizeClassSubsetFraction",
  "HalfSaturationConstantFraction",
  "PreferredPreyVolumeRatio",
  "PreferenceFunctionWidth",
  "AssimilationEfficiency",
  "FractionalMetabolicExpense",
  "MetabolicIndex",
  "MutationProbability",
  "MutationStandardDeviation",
};

inline std::vector<std::string> getParamNames() {
  return std::vector<std::string>(std::begin(consts::kParamNames), std::end(consts::kParamNames));
}

}  // namespace Constants

#endif
