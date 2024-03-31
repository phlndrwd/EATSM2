/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "Parameters.h"
#include "Strings.h"

Parameters* Parameters::this_ = nullptr;

Parameters::Parameters() {
  parametersInitialised_ = {false};
}

Parameters::~Parameters() {
  if (this_ != nullptr) {
    delete this_;
  }
}

Parameters* Parameters::Get() {
  if (this_ == nullptr) {
    this_ = new Parameters();
  }
  return this_;
}

bool Parameters::initialise(const std::vector<std::vector<std::string>>& rawInputParameterData) {
  if (rawInputParameterData.size() > 0) {
    for (unsigned rowIndex = 0; rowIndex < rawInputParameterData.size(); ++rowIndex) {
      std::string parameterName = Strings::toLowercase(rawInputParameterData[rowIndex][enums::eParameterName]);
      double parameterValue = Strings::stringToNumber(rawInputParameterData[rowIndex][enums::eParameterValue]);

      if (parameterName == "randomseed")
        setRandomSeed(parameterValue);
      else if (parameterName == "runtimeinseconds")
        setRunTimeInSeconds(parameterValue);
      else if (parameterName == "samplingrate")
        setSamplingRate(parameterValue);
      else if (parameterName == "numberofsizeclasses")
        setNumberOfSizeClasses(parameterValue);

      else if (parameterName == "readmodelstate")
        setReadModelState(parameterValue);
      else if (parameterName == "writemodelstate")
        setWriteModelState(parameterValue);
      else if (parameterName == "uselinearfeeding")
        setUseLinearFeeding(parameterValue);

      else if (parameterName == "initialautotrophicvolume")
        setInitialAutotrophicVolume(parameterValue);
      else if (parameterName == "initialheterotrophicvolume")
        setInitialHeterotrophicVolume(parameterValue);
      else if (parameterName == "minimumheterotrophicvolume")
        setMinimumHeterotrophicVolume(parameterValue);

      else if (parameterName == "smallestindividualvolume")
        setSmallestIndividualVolume(parameterValue);
      else if (parameterName == "largestindividualvolume")
        setLargestIndividualVolume(parameterValue);
      else if (parameterName == "preferredpreyvolumeratio")
        setPreferredPreyVolumeRatio(parameterValue);
      else if (parameterName == "preferencefunctionwidth")
        setPreferenceFunctionWidth(parameterValue);
      else if (parameterName == "sizeclasssubsetfraction")
        setSizeClassSubsetFraction(parameterValue);
      else if (parameterName == "halfsaturationconstantfraction")
        setHalfSaturationConstantFraction(parameterValue);

      else if (parameterName == "assimilationefficiency")
        setAssimilationEfficiency(parameterValue);
      else if (parameterName == "fractionalmetabolicexpense")
        setFractionalMetabolicExpense(parameterValue);

      else if (parameterName == "metabolicindex")
        setMetabolicIndex(parameterValue);
      else if (parameterName == "mutationprobability")
        setMutationProbability(parameterValue);
      else if (parameterName == "mutationstandarddeviation")
        setMutationStandardDeviation(parameterValue);
    }

    return isInitialised();
  } else
    return false;
}

bool Parameters::isInitialised() {
  bool isInitialised = true;
  for (unsigned i = 0; i < enums::eNumberOfParamters; ++i)
    if (parametersInitialised_[i] == false) isInitialised = false;

  return isInitialised;
}

unsigned Parameters::getRunTimeInSeconds() {
  return runTimeInSeconds_;
}

unsigned Parameters::getRandomSeed() {
  return randomSeed_;
}

unsigned Parameters::getSamplingRate() {
  return samplingRate_;
}

unsigned Parameters::getNumberOfSizeClasses() {
  return numberOfSizeClasses_;
}

bool Parameters::getReadModelState() {
  return readModelState_;
}

bool Parameters::getWriteModelState() {
  return writeModelState_;
}

bool Parameters::getUseLinearFeeding() {
  return useLinearFeeding_;
}

double Parameters::getInitialAutotrophVolume() {
  return initialAutotrophicVolume_;
}

double Parameters::getInitialHeterotrophVolume() {
  return initialHeterotrophicVolume_;
}

double Parameters::getMinimumHeterotrophicVolume() {
  return minimumHeterotrophicVolume_;
}

double Parameters::getSmallestIndividualVolume() {
  return smallestIndividualVolume_;
}

double Parameters::getLargestIndividualVolume() {
  return largestIndividualVolume_;
}

unsigned Parameters::getPreferredPreyVolumeRatio() {
  return preferredPreyVolumeRatio_;
}

double Parameters::getPreferenceFunctionWidth() {
  return preferenceFunctionWidth_;
}

double Parameters::getSizeClassSubsetFraction() {
  return sizeClassSubsetFraction_;
}

double Parameters::getHalfSaturationConstantFraction() {
  return halfSaturationConstantFraction_;
}

double Parameters::getAssimilationEfficiency() {
  return assimilationEfficiency_;
}

double Parameters::getFractionalMetabolicExpense() {
  return fractionalMetabolicExpense_;
}

double Parameters::getMetabolicIndex() {
  return metabolicIndex_;
}

double Parameters::getMutationProbability() {
  return mutationProbability_;
}

double Parameters::getMutationStandardDeviation() {
  return mutationStandardDeviation_;
}

void Parameters::setRandomSeed(const unsigned randomNumberSeed) {
  randomSeed_ = randomNumberSeed;
  parametersInitialised_[enums::eRandomSeed] = true;
}

void Parameters::setRunTimeInSeconds(const unsigned runTimeInSeconds) {
  runTimeInSeconds_ = runTimeInSeconds;
  parametersInitialised_[enums::eRunTimeInSeconds] = true;
}

void Parameters::setSamplingRate(const unsigned samplingRate) {
  samplingRate_ = samplingRate;
  parametersInitialised_[enums::eSamplingRate] = true;
}

void Parameters::setNumberOfSizeClasses(const unsigned numberOfSizeClasses) {
  numberOfSizeClasses_ = numberOfSizeClasses;
  parametersInitialised_[enums::eNumberOfSizeClasses] = true;
}

void Parameters::setReadModelState(const bool createNewPopulation) {
  readModelState_ = createNewPopulation;
  parametersInitialised_[enums::eReadModelState] = true;
}

void Parameters::setWriteModelState(const bool writeModelState) {
  writeModelState_ = writeModelState;
  parametersInitialised_[enums::eWriteModelState] = true;
}

void Parameters::setUseLinearFeeding(const bool useLinearFeeding) {
  useLinearFeeding_ = useLinearFeeding;
  parametersInitialised_[enums::eUseLinearFeeding] = true;
}

void Parameters::setInitialAutotrophicVolume(const double initialAutotrophicVolume) {
  initialAutotrophicVolume_ = initialAutotrophicVolume;
  parametersInitialised_[enums::eInitialAutotrophicVolume] = true;
}

void Parameters::setInitialHeterotrophicVolume(const double initialHeterotrophicVolume) {
  initialHeterotrophicVolume_ = initialHeterotrophicVolume;
  parametersInitialised_[enums::eInitialHeterotrophicVolume] = true;
}

void Parameters::setMinimumHeterotrophicVolume(const double minimumHeterotrophicVolume) {
  minimumHeterotrophicVolume_ = minimumHeterotrophicVolume;
  parametersInitialised_[enums::eMinimumHeterotrophicVolume] = true;
}

void Parameters::setSmallestIndividualVolume(double smallestIndividualVolume) {
  smallestIndividualVolume_ = smallestIndividualVolume;
  parametersInitialised_[enums::eSmallestIndividualVolume] = true;
}

void Parameters::setLargestIndividualVolume(double largestIndividualVolume) {
  largestIndividualVolume_ = largestIndividualVolume;
  parametersInitialised_[enums::eLargestIndividualVolume] = true;
}

void Parameters::setPreferredPreyVolumeRatio(const unsigned preferredPreyVolumeRatio) {
  preferredPreyVolumeRatio_ = preferredPreyVolumeRatio;
  parametersInitialised_[enums::ePreferredPreyVolumeRatio] = true;
}

void Parameters::setPreferenceFunctionWidth(const double preferenceFunctionWidth) {
  preferenceFunctionWidth_ = preferenceFunctionWidth;
  parametersInitialised_[enums::ePreferenceFunctionWidth] = true;
}

void Parameters::setSizeClassSubsetFraction(const double sizeClassSubsetFraction) {
  sizeClassSubsetFraction_ = sizeClassSubsetFraction;
  parametersInitialised_[enums::eSizeClassSubsetFraction] = true;
}

void Parameters::setHalfSaturationConstantFraction(const double halfSaturationConstantFraction) {
  halfSaturationConstantFraction_ = halfSaturationConstantFraction;
  parametersInitialised_[enums::eHalfSaturationConstantFraction] = true;
}

void Parameters::setAssimilationEfficiency(const double assimilationEfficiency) {
  assimilationEfficiency_ = assimilationEfficiency;
  parametersInitialised_[enums::eAssimilationEfficiency] = true;
}

void Parameters::setFractionalMetabolicExpense(const double fractionalMetabolicExpense) {
  fractionalMetabolicExpense_ = fractionalMetabolicExpense;
  parametersInitialised_[enums::eFractionalMetabolicExpense] = true;
}

void Parameters::setMetabolicIndex(const double metabolicIndex) {
  metabolicIndex_ = metabolicIndex;
  parametersInitialised_[enums::eMetabolicIndex] = true;
}

void Parameters::setMutationProbability(const double mutationProbability) {
  mutationProbability_ = mutationProbability;
  parametersInitialised_[enums::eMutationProbability] = true;
}

void Parameters::setMutationStandardDeviation(const double mutationStandardDeviation) {
  mutationStandardDeviation_ = mutationStandardDeviation;
  parametersInitialised_[enums::eMutationStandardDeviation] = true;
}
