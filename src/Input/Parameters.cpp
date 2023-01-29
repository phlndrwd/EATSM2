#include "Parameters.h"

#include <cmath>

#include "Constants.h"
#include "HeterotrophProcessor.h"
#include "Strings.h"

Parameters* Parameters::this_ = nullptr;

Parameters::Parameters() {
  parametersInitialised_ = {false};
}

Parameters::~Parameters() {
  if (this_ != nullptr) {
    for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
      interSizeClassPreferenceMatrix_[sizeClassIndex].clear();
      interSizeClassVolumeMatrix_[sizeClassIndex].clear();
    }
    interSizeClassPreferenceMatrix_.clear();
    interSizeClassVolumeMatrix_.clear();

    remainingVolumes_.clear();
    linearFeedingDenominators_.clear();
    halfSaturationConstants_.clear();
    sizeClassBoundaries_.clear();
    sizeClassMidPoints_.clear();

    delete this_;
  }
}

Parameters* Parameters::Get() {
  if (this_ == nullptr) this_ = new Parameters();

  return this_;
}

bool Parameters::initialise(const std::vector<std::vector<std::string>>& rawInputParameterData) {
  if (rawInputParameterData.size() > 0) {
    for (unsigned rowIndex = 0; rowIndex < rawInputParameterData.size(); ++rowIndex) {
      std::string parameterName = Strings::toLowercase(rawInputParameterData[rowIndex][constants::eParameterName]);
      double parameterValue = Strings::stringToNumber(rawInputParameterData[rowIndex][constants::eParameterValue]);

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
    calculateParameters();

    return isInitialised();
  } else
    return false;
}

void Parameters::calculateParameters() {
  maximumSizeClassPopulations_.resize(numberOfSizeClasses_, 0);
  remainingVolumes_.resize(numberOfSizeClasses_);
  linearFeedingDenominators_.resize(numberOfSizeClasses_);
  halfSaturationConstants_.resize(numberOfSizeClasses_);
  sizeClassMidPoints_.resize(numberOfSizeClasses_);
  sizeClassBoundaries_.resize(numberOfSizeClasses_ + 1);

  smallestVolumeExponent_ = std::log10(smallestIndividualVolume_);
  largestVolumeExponent_ = std::log10(largestIndividualVolume_);

  totalVolume_ = initialAutotrophicVolume_ + initialHeterotrophicVolume_;

  double sizeClassExponentIncrement = (largestVolumeExponent_ - smallestVolumeExponent_) / numberOfSizeClasses_;

  for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
    double sizeClassMidPointExponent = smallestVolumeExponent_ + ((sizeClassIndex + 0.5) * sizeClassExponentIncrement);
    double sizeClassBoundaryExponent = smallestVolumeExponent_ + (sizeClassIndex * sizeClassExponentIncrement);

    sizeClassBoundaries_[sizeClassIndex] = std::pow(10, sizeClassBoundaryExponent);
    sizeClassMidPoints_[sizeClassIndex] = std::pow(10, sizeClassMidPointExponent);

    remainingVolumes_[sizeClassIndex] = totalVolume_ - sizeClassMidPoints_[sizeClassIndex];
    linearFeedingDenominators_[sizeClassIndex] =
        (2 * halfSaturationConstantFraction_) * remainingVolumes_[sizeClassIndex];
    halfSaturationConstants_[sizeClassIndex] = halfSaturationConstantFraction_ * remainingVolumes_[sizeClassIndex];
    maximumSizeClassPopulations_[sizeClassIndex] = std::ceil(totalVolume_ / sizeClassMidPoints_[sizeClassIndex]);
  }
  double sizeClassBoundaryExponent = smallestVolumeExponent_ + (numberOfSizeClasses_ * sizeClassExponentIncrement);
  sizeClassBoundaries_[numberOfSizeClasses_] = std::pow(10, sizeClassBoundaryExponent);

  HeterotrophProcessor temporaryHeterotrophProcessor;
  autotrophSizeClassIndex_ = temporaryHeterotrophProcessor.findSizeClassIndexFromVolume(smallestIndividualVolume_);

  interSizeClassPreferenceMatrix_.resize(numberOfSizeClasses_);
  interSizeClassVolumeMatrix_.resize(numberOfSizeClasses_);

  for (unsigned subjectIndex = 0; subjectIndex < numberOfSizeClasses_; ++subjectIndex) {
    double subjectVolumeMean = sizeClassMidPoints_[subjectIndex];
    double preferenceSum = 0;

    for (unsigned referenceIndex = 0; referenceIndex < numberOfSizeClasses_; ++referenceIndex) {
      double referenceVolumeMean = sizeClassMidPoints_[referenceIndex];

      double preferenceForReferenceSizeClass =
          temporaryHeterotrophProcessor.calculatePreferenceForPrey(subjectVolumeMean, referenceVolumeMean);

      preferenceSum += preferenceForReferenceSizeClass;

      interSizeClassPreferenceMatrix_[subjectIndex].push_back(preferenceForReferenceSizeClass);
      interSizeClassVolumeMatrix_[subjectIndex].push_back(preferenceForReferenceSizeClass * referenceVolumeMean);
    }
  }
}

bool Parameters::isInitialised() {
  bool isInitialised = true;
  for (unsigned i = 0; i < constants::eMutationStandardDeviation + 1; ++i)
    if (parametersInitialised_[i] == false) isInitialised = false;

  return isInitialised;
}

unsigned& Parameters::getRunTimeInSeconds() {
  return runTimeInSeconds_;
}

unsigned& Parameters::getRandomSeed() {
  return randomSeed_;
}

unsigned& Parameters::getSamplingRate() {
  return samplingRate_;
}

unsigned& Parameters::getNumberOfSizeClasses() {
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

double& Parameters::getInitialAutotrophVolume() {
  return initialAutotrophicVolume_;
}

double& Parameters::getInitialHeterotrophVolume() {
  return initialHeterotrophicVolume_;
}

double& Parameters::getMinimumHeterotrophicVolume() {
  return minimumHeterotrophicVolume_;
}

double& Parameters::getSmallestIndividualVolume() {
  return smallestIndividualVolume_;
}

double& Parameters::getLargestIndividualVolume() {
  return largestIndividualVolume_;
}

unsigned& Parameters::getPreferredPreyVolumeRatio() {
  return preferredPreyVolumeRatio_;
}

double& Parameters::getPreferenceFunctionWidth() {
  return preferenceFunctionWidth_;
}

double& Parameters::getSizeClassSubsetFraction() {
  return sizeClassSubsetFraction_;
}

double& Parameters::getHalfSaturationConstantFraction() {
  return halfSaturationConstantFraction_;
}

double& Parameters::getAssimilationEfficiency() {
  return assimilationEfficiency_;
}

double& Parameters::getFractionalMetabolicExpense() {
  return fractionalMetabolicExpense_;
}

double& Parameters::getMetabolicIndex() {
  return metabolicIndex_;
}

double& Parameters::getMutationProbability() {
  return mutationProbability_;
}

double& Parameters::getMutationStandardDeviation() {
  return mutationStandardDeviation_;
}

unsigned& Parameters::getAutotrophSizeClassIndex() {
  return autotrophSizeClassIndex_;
}

double& Parameters::getSmallestVolumeExponent() {
  return smallestVolumeExponent_;
}

double& Parameters::getLargestVolumeExponent() {
  return largestVolumeExponent_;
}

double& Parameters::getSizeClassBoundary(const unsigned index) {
  return sizeClassBoundaries_[index];
}

double& Parameters::getSizeClassMidPoint(const unsigned index) {
  return sizeClassMidPoints_[index];
}

const std::vector<double>& Parameters::getSizeClassBoundaries() {
  return sizeClassBoundaries_;
}

const std::vector<double>& Parameters::getSizeClassMidPoints() {
  return sizeClassMidPoints_;
}

const std::vector<double>& Parameters::getLinearFeedingDenominators() {
  return linearFeedingDenominators_;
}

const std::vector<double>& Parameters::getHalfSaturationConstants() {
  return halfSaturationConstants_;
}

const std::vector<unsigned>& Parameters::getMaximumSizeClassPopulations() {
  return maximumSizeClassPopulations_;
}

double& Parameters::getInterSizeClassPreference(const unsigned predatorIndex, const unsigned preyIndex) {
  return interSizeClassPreferenceMatrix_[predatorIndex][preyIndex];
}

double& Parameters::getInterSizeClassVolume(const unsigned predatorIndex, const unsigned preyIndex) {
  return interSizeClassVolumeMatrix_[predatorIndex][preyIndex];
}

double& Parameters::getTotalVolume() { return totalVolume_; }

unsigned& Parameters::getMaximumSizeClassPopulation(const unsigned sizeClassIndex) {
  return maximumSizeClassPopulations_[sizeClassIndex];
}

double& Parameters::getRemainingVolume(const unsigned sizeClassIndex) { return remainingVolumes_[sizeClassIndex]; }

double& Parameters::getLinearFeedingDenominator(const unsigned sizeClassIndex) {
  return linearFeedingDenominators_[sizeClassIndex];
}

double& Parameters::getHalfSaturationConstant(const unsigned sizeClassIndex) {
  return halfSaturationConstants_[sizeClassIndex];
}

const std::vector<double>& Parameters::getInterSizeClassPreferenceVector(const unsigned index) const {
  return interSizeClassPreferenceMatrix_[index];
}

const std::vector<double>& Parameters::getInterSizeClassVolumeVector(const unsigned index) const {
  return interSizeClassVolumeMatrix_[index];
}

const std::vector<std::vector<double>>& Parameters::getInterSizeClassPreferenceMatrix() const {
  return interSizeClassPreferenceMatrix_;
}

const std::vector<std::vector<double>>& Parameters::getInterSizeClassVolumeMatrix() const { return interSizeClassVolumeMatrix_; }

void Parameters::setRandomSeed(const unsigned randomNumberSeed) {
  randomSeed_ = randomNumberSeed;
  parametersInitialised_[constants::eRandomSeed] = true;
}

void Parameters::setRunTimeInSeconds(const unsigned runTimeInSeconds) {
  runTimeInSeconds_ = runTimeInSeconds;
  parametersInitialised_[constants::eRunTimeInSeconds] = true;
}

void Parameters::setSamplingRate(const unsigned samplingRate) {
  samplingRate_ = samplingRate;
  parametersInitialised_[constants::eSamplingRate] = true;
}

void Parameters::setNumberOfSizeClasses(const unsigned numberOfSizeClasses) {
  numberOfSizeClasses_ = numberOfSizeClasses;
  parametersInitialised_[constants::eNumberOfSizeClasses] = true;
}

void Parameters::setReadModelState(const bool createNewPopulation) {
  readModelState_ = createNewPopulation;
  parametersInitialised_[constants::eReadModelState] = true;
}

void Parameters::setWriteModelState(const bool writeModelState) {
  writeModelState_ = writeModelState;
  parametersInitialised_[constants::eWriteModelState] = true;
}

void Parameters::setUseLinearFeeding(const bool useLinearFeeding) {
  useLinearFeeding_ = useLinearFeeding;
  parametersInitialised_[constants::eUseLinearFeeding] = true;
}

void Parameters::setInitialAutotrophicVolume(const double initialAutotrophicVolume) {
  initialAutotrophicVolume_ = initialAutotrophicVolume;
  parametersInitialised_[constants::eInitialAutotrophicVolume] = true;
}

void Parameters::setInitialHeterotrophicVolume(const double initialHeterotrophicVolume) {
  initialHeterotrophicVolume_ = initialHeterotrophicVolume;
  parametersInitialised_[constants::eInitialHeterotrophicVolume] = true;
}

void Parameters::setMinimumHeterotrophicVolume(const double minimumHeterotrophicVolume) {
  minimumHeterotrophicVolume_ = minimumHeterotrophicVolume;
  parametersInitialised_[constants::eMinimumHeterotrophicVolume] = true;
}

void Parameters::setSmallestIndividualVolume(double smallestIndividualVolume) {
  smallestIndividualVolume_ = smallestIndividualVolume;
  parametersInitialised_[constants::eSmallestIndividualVolume] = true;
}

void Parameters::setLargestIndividualVolume(double largestIndividualVolume) {
  largestIndividualVolume_ = largestIndividualVolume;
  parametersInitialised_[constants::eLargestIndividualVolume] = true;
}

void Parameters::setPreferredPreyVolumeRatio(const unsigned preferredPreyVolumeRatio) {
  preferredPreyVolumeRatio_ = preferredPreyVolumeRatio;
  parametersInitialised_[constants::ePreferredPreyVolumeRatio] = true;
}

void Parameters::setPreferenceFunctionWidth(const double preferenceFunctionWidth) {
  preferenceFunctionWidth_ = preferenceFunctionWidth;
  parametersInitialised_[constants::ePreferenceFunctionWidth] = true;
}

void Parameters::setSizeClassSubsetFraction(const double sizeClassSubsetFraction) {
  sizeClassSubsetFraction_ = sizeClassSubsetFraction;
  parametersInitialised_[constants::eSizeClassSubsetFraction] = true;
}

void Parameters::setHalfSaturationConstantFraction(const double halfSaturationConstantFraction) {
  halfSaturationConstantFraction_ = halfSaturationConstantFraction;
  parametersInitialised_[constants::eHalfSaturationConstantFraction] = true;
}

void Parameters::setAssimilationEfficiency(const double assimilationEfficiency) {
  assimilationEfficiency_ = assimilationEfficiency;
  parametersInitialised_[constants::eAssimilationEfficiency] = true;
}

void Parameters::setFractionalMetabolicExpense(const double fractionalMetabolicExpense) {
  fractionalMetabolicExpense_ = fractionalMetabolicExpense;
  parametersInitialised_[constants::eFractionalMetabolicExpense] = true;
}

void Parameters::setMetabolicIndex(const double metabolicIndex) {
  metabolicIndex_ = metabolicIndex;
  parametersInitialised_[constants::eMetabolicIndex] = true;
}

void Parameters::setMutationProbability(const double mutationProbability) {
  mutationProbability_ = mutationProbability;
  parametersInitialised_[constants::eMutationProbability] = true;
}

void Parameters::setMutationStandardDeviation(const double mutationStandardDeviation) {
  mutationStandardDeviation_ = mutationStandardDeviation;
  parametersInitialised_[constants::eMutationStandardDeviation] = true;
}
