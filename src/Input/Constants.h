#ifndef CONSTANTS
#define CONSTANTS

#include <string>

namespace constants {

// Input data file enums
enum eInputParameters {
  eRandomSeed,
  eRunTimeInSeconds,
  eSamplingRate,
  eNumberOfSizeClasses,
  eReadModelState,
  eWriteModelState,
  eUseLinearFeeding,
  eInitialAutotrophicVolume,
  eInitialHeterotrophicVolume,
  eMinimumHeterotrophicVolume,
  eSmallestIndividualVolume,
  eLargestIndividualVolume,
  eSizeClassSubsetFraction,
  eHalfSaturationConstantFraction,
  ePreferredPreyVolumeRatio,
  ePreferenceFunctionWidth,
  eAssimilationEfficiency,
  eFractionalMetabolicExpense,
  eMetabolicIndex,
  eMutationProbability,
  eMutationStandardDeviation,
  eNumberOfParamters
};

enum eInputParametersMetadata {
  eParameterName,
  eParameterValue
};

enum eOutputControlParameters {
  eDatumName,
  eDatumType
};

enum eHeritableTraitIndices {
  eVolume
};

// Model enums

enum eMovementDirection {
  eNoMovement,
  eMoveUp,
  eMoveDown
};

const unsigned kStateLineNutrientVol = 0;
const unsigned kStateLineAutotrophVol = 1;
const unsigned kStateLineFirstHeterotroph = 2;

const std::string kConfigurationDirectory = "./input/";
const std::string kOutputDirectoryName = "output";

const std::string kInputParametersFileName = "Parameters.csv";
const std::string kOutputParametersFileName = "Variables.csv";
const std::string kInitialStateFileName = "InitialState.csv";
const std::string kModelStateFileName = "State.csv";
const std::string kFileNameExtension = ".csv";

const std::string kVectorDatumTypeName = "vector";
const std::string kMatrixDatumTypeName = "matrix";

const unsigned kMaximumNumberOfTrophicLevels = 11;  // 0 = unclassified, 1 = primary, etc.
const int kMissingValue = -9999;

const unsigned kDateTimeBufferSize = 25;
const unsigned kOutputFolderPermissions = 0777;

const unsigned kReproductionFactor = 2;
const double kReproductionMultiplier = 1 / (double)kReproductionFactor;  // Equivalent to 1 / 2
const double kMinimumFractionalVolume = 0.5;

const unsigned kSecondsInAMinute = 60;
const unsigned kMinutesInAnHour = 60;
const unsigned kHoursInADay = 24;

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
