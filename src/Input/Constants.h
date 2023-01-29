#ifndef CONSTANTS
#define CONSTANTS

#include <string>

namespace Constants {

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
  eMutationStandardDeviation
};

enum eInputParametersMetadata { eParameterName, eParameterValue };

enum eOutputControlParameters { eDatumName, eDatumType };

enum eHeritableTraitIndices { eVolume };

// Model enums

enum eMovementDirection { eNoMovement, eMoveUp, eMoveDown };

const unsigned cStateLineNutrientVol = 0;
const unsigned cStateLineAutotrophVol = 1;
const unsigned cStateLineFirstHeterotroph = 2;

const std::string cConfigurationDirectory = "./input/";
const std::string cOutputDirectoryName = "output";

const std::string cInputParametersFileName = "Parameters.csv";
const std::string cOutputParametersFileName = "Variables.csv";
const std::string cInitialStateFileName = "InitialState.csv";
const std::string cModelStateFileName = "State.csv";
const std::string cFileNameExtension = ".csv";

const std::string cTagFileName = "Tag_";
const std::string cAttributesFileName = "Attributes.csv";
const std::string cHerbivoryEventsFileName = "HerbivoryEvents.csv";
const std::string cCarnivoryEventsFileName = "CarnivoryEvents.csv";

const std::string cVectorDatumTypeName = "vector";
const std::string cMatrixDatumTypeName = "matrix";

const unsigned cMaximumNumberOfTrophicLevels = 11;  // 0 = unclassified, 1 = primary, etc.
const int cMissingValue = -9999;

const unsigned cDateTimeBufferSize = 25;
const unsigned cOutputFolderPermissions = 0777;

const unsigned cReproductionFactor = 2;
const double cReproductionMultiplier = 1 / (double)cReproductionFactor;  // Equivalent to 1 / 2
const double cMinimumFractionalVolume = 0.5;

const unsigned cSecondsInAMinute = 60;
const unsigned cMinutesInAnHour = 60;
const unsigned cHoursInADay = 24;

const std::string cCompleteDateFormat = "%c";
const std::string cDataSetNameFormat = "%Y-%m-%d_%H-%M-%S";

const char cFolderDelimiter = '/';
const char cWhiteSpaceCharacter = ' ';
const char cDataDelimiterValue = ',';
const char cCommentCharacter = '#';

const std::string cParameterFileCommand = "-p";
const std::string cStateFileCommand = "-s";
const std::string cRestartCommand = "-r";
const std::string cVersionCommand = "-v";

const std::string cSystemName = "EATSM2";
const std::string cSystemVersion = "0.0.1";
const std::string cSystemDate = "29/01/2023";
const std::string cSystemTime = "00:00";
}  // namespace Constants

#endif
