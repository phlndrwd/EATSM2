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

Parameters::Parameters() {
  parametersInitialised_ = {false};
}

Parameters::~Parameters() {}

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

const unsigned& Parameters::getRunTimeInSeconds() const {
  return runTimeInSeconds_;
}

const unsigned& Parameters::getRandomSeed() const {
  return randomSeed_;
}

const unsigned& Parameters::getSamplingRate() const {
  return samplingRate_;
}

const unsigned& Parameters::getNumberOfSizeClasses() const {
  return numberOfSizeClasses_;
}

const bool& Parameters::getReadModelState() const {
  return readModelState_;
}

const bool& Parameters::getWriteModelState() const {
  return writeModelState_;
}

const bool& Parameters::getUseLinearFeeding() const {
  return useLinearFeeding_;
}

const double& Parameters::getInitialAutotrophVolume() const {
  return initialAutotrophicVolume_;
}

const double& Parameters::getInitialHeterotrophVolume() const {
  return initialHeterotrophicVolume_;
}

const double& Parameters::getMinimumHeterotrophicVolume() const {
  return minimumHeterotrophicVolume_;
}

const double& Parameters::getSmallestIndividualVolume() const {
  return smallestIndividualVolume_;
}

const double& Parameters::getLargestIndividualVolume() const {
  return largestIndividualVolume_;
}

const unsigned& Parameters::getPreferredPreyVolumeRatio() const {
  return preferredPreyVolumeRatio_;
}

const double& Parameters::getPreferenceFunctionWidth() const {
  return preferenceFunctionWidth_;
}

const double& Parameters::getSizeClassSubsetFraction() const {
  return sizeClassSubsetFraction_;
}

const double& Parameters::getHalfSaturationConstantFraction() const {
  return halfSaturationConstantFraction_;
}

const double& Parameters::getAssimilationEfficiency() const {
  return assimilationEfficiency_;
}

const double& Parameters::getFractionalMetabolicExpense() const {
  return fractionalMetabolicExpense_;
}

const double& Parameters::getMetabolicIndex() const {
  return metabolicIndex_;
}

const double& Parameters::getMutationProbability() const {
  return mutationProbability_;
}

const double& Parameters::getMutationStandardDeviation() const {
  return mutationStandardDeviation_;
}

void Parameters::setRandomSeed(const unsigned& randomNumberSeed) {
  randomSeed_ = randomNumberSeed;
  parametersInitialised_[enums::eRandomSeed] = true;
}

void Parameters::setRunTimeInSeconds(const unsigned& runTimeInSeconds) {
  runTimeInSeconds_ = runTimeInSeconds;
  parametersInitialised_[enums::eRunTimeInSeconds] = true;
}

void Parameters::setSamplingRate(const unsigned& samplingRate) {
  samplingRate_ = samplingRate;
  parametersInitialised_[enums::eSamplingRate] = true;
}

void Parameters::setNumberOfSizeClasses(const unsigned& numberOfSizeClasses) {
  numberOfSizeClasses_ = numberOfSizeClasses;
  parametersInitialised_[enums::eNumberOfSizeClasses] = true;
}

void Parameters::setReadModelState(const bool& createNewPopulation) {
  readModelState_ = createNewPopulation;
  parametersInitialised_[enums::eReadModelState] = true;
}

void Parameters::setWriteModelState(const bool& writeModelState) {
  writeModelState_ = writeModelState;
  parametersInitialised_[enums::eWriteModelState] = true;
}

void Parameters::setUseLinearFeeding(const bool& useLinearFeeding) {
  useLinearFeeding_ = useLinearFeeding;
  parametersInitialised_[enums::eUseLinearFeeding] = true;
}

void Parameters::setInitialAutotrophicVolume(const double& initialAutotrophicVolume) {
  initialAutotrophicVolume_ = initialAutotrophicVolume;
  parametersInitialised_[enums::eInitialAutotrophicVolume] = true;
}

void Parameters::setInitialHeterotrophicVolume(const double& initialHeterotrophicVolume) {
  initialHeterotrophicVolume_ = initialHeterotrophicVolume;
  parametersInitialised_[enums::eInitialHeterotrophicVolume] = true;
}

void Parameters::setMinimumHeterotrophicVolume(const double& minimumHeterotrophicVolume) {
  minimumHeterotrophicVolume_ = minimumHeterotrophicVolume;
  parametersInitialised_[enums::eMinimumHeterotrophicVolume] = true;
}

void Parameters::setSmallestIndividualVolume(const double& smallestIndividualVolume) {
  smallestIndividualVolume_ = smallestIndividualVolume;
  parametersInitialised_[enums::eSmallestIndividualVolume] = true;
}

void Parameters::setLargestIndividualVolume(const double& largestIndividualVolume) {
  largestIndividualVolume_ = largestIndividualVolume;
  parametersInitialised_[enums::eLargestIndividualVolume] = true;
}

void Parameters::setPreferredPreyVolumeRatio(const unsigned& preferredPreyVolumeRatio) {
  preferredPreyVolumeRatio_ = preferredPreyVolumeRatio;
  parametersInitialised_[enums::ePreferredPreyVolumeRatio] = true;
}

void Parameters::setPreferenceFunctionWidth(const double& preferenceFunctionWidth) {
  preferenceFunctionWidth_ = preferenceFunctionWidth;
  parametersInitialised_[enums::ePreferenceFunctionWidth] = true;
}

void Parameters::setSizeClassSubsetFraction(const double& sizeClassSubsetFraction) {
  sizeClassSubsetFraction_ = sizeClassSubsetFraction;
  parametersInitialised_[enums::eSizeClassSubsetFraction] = true;
}

void Parameters::setHalfSaturationConstantFraction(const double& halfSaturationConstantFraction) {
  halfSaturationConstantFraction_ = halfSaturationConstantFraction;
  parametersInitialised_[enums::eHalfSaturationConstantFraction] = true;
}

void Parameters::setAssimilationEfficiency(const double& assimilationEfficiency) {
  assimilationEfficiency_ = assimilationEfficiency;
  parametersInitialised_[enums::eAssimilationEfficiency] = true;
}

void Parameters::setFractionalMetabolicExpense(const double& fractionalMetabolicExpense) {
  fractionalMetabolicExpense_ = fractionalMetabolicExpense;
  parametersInitialised_[enums::eFractionalMetabolicExpense] = true;
}

void Parameters::setMetabolicIndex(const double& metabolicIndex) {
  metabolicIndex_ = metabolicIndex;
  parametersInitialised_[enums::eMetabolicIndex] = true;
}

void Parameters::setMutationProbability(const double& mutationProbability) {
  mutationProbability_ = mutationProbability;
  parametersInitialised_[enums::eMutationProbability] = true;
}

void Parameters::setMutationStandardDeviation(const double& mutationStandardDeviation) {
  mutationStandardDeviation_ = mutationStandardDeviation;
  parametersInitialised_[enums::eMutationStandardDeviation] = true;
}
