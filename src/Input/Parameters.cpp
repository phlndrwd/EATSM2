#include "Parameters.h"

#include <cmath>

#include "Constants.h"
#include "HeterotrophProcessor.h"
#include "Strings.h"

Parameters* Parameters::mThis = nullptr;

Parameters::Parameters() { mParametersInitialised = {false}; }

Parameters::~Parameters() {
  if (mThis != nullptr) {
    for (unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex) {
      mInterSizeClassPreferenceMatrix[sizeClassIndex].clear();
      mInterSizeClassVolumeMatrix[sizeClassIndex].clear();
    }
    mInterSizeClassPreferenceMatrix.clear();
    mInterSizeClassVolumeMatrix.clear();

    mRemainingVolumes.clear();
    mLinearFeedingDenominators.clear();
    mHalfSaturationConstants.clear();
    mSizeClassBoundaries.clear();
    mSizeClassMidPoints.clear();

    delete mThis;
  }
}

Parameters* Parameters::Get() {
  if (mThis == nullptr) mThis = new Parameters();

  return mThis;
}

bool Parameters::Initialise(const std::vector<std::vector<std::string>>& rawInputParameterData) {
  if (rawInputParameterData.size() > 0) {
    for (unsigned rowIndex = 0; rowIndex < rawInputParameterData.size(); ++rowIndex) {
      std::string parameterName = Strings::ToLowercase(rawInputParameterData[rowIndex][Constants::eParameterName]);
      double parameterValue = Strings::StringToNumber(rawInputParameterData[rowIndex][Constants::eParameterValue]);

      if (parameterName == "randomseed")
        SetRandomSeed(parameterValue);
      else if (parameterName == "runtimeinseconds")
        SetRunTimeInSeconds(parameterValue);
      else if (parameterName == "samplingrate")
        SetSamplingRate(parameterValue);
      else if (parameterName == "numberofsizeclasses")
        SetNumberOfSizeClasses(parameterValue);

      else if (parameterName == "readmodelstate")
        SetReadModelState(parameterValue);
      else if (parameterName == "writemodelstate")
        SetWriteModelState(parameterValue);
      else if (parameterName == "uselinearfeeding")
        SetUseLinearFeeding(parameterValue);

      else if (parameterName == "initialautotrophicvolume")
        SetInitialAutotrophicVolume(parameterValue);
      else if (parameterName == "initialheterotrophicvolume")
        SetInitialHeterotrophicVolume(parameterValue);
      else if (parameterName == "minimumheterotrophicvolume")
        SetMinimumHeterotrophicVolume(parameterValue);

      else if (parameterName == "smallestindividualvolume")
        SetSmallestIndividualVolume(parameterValue);
      else if (parameterName == "largestindividualvolume")
        SetLargestIndividualVolume(parameterValue);
      else if (parameterName == "preferredpreyvolumeratio")
        SetPreferredPreyVolumeRatio(parameterValue);
      else if (parameterName == "preferencefunctionwidth")
        SetPreferenceFunctionWidth(parameterValue);
      else if (parameterName == "sizeclasssubsetfraction")
        SetSizeClassSubsetFraction(parameterValue);
      else if (parameterName == "halfsaturationconstantfraction")
        SetHalfSaturationConstantFraction(parameterValue);

      else if (parameterName == "assimilationefficiency")
        SetAssimilationEfficiency(parameterValue);
      else if (parameterName == "fractionalmetabolicexpense")
        SetFractionalMetabolicExpense(parameterValue);

      else if (parameterName == "metabolicindex")
        SetMetabolicIndex(parameterValue);
      else if (parameterName == "mutationprobability")
        SetMutationProbability(parameterValue);
      else if (parameterName == "mutationstandarddeviation")
        SetMutationStandardDeviation(parameterValue);
    }
    CalculateParameters();

    return IsInitialised();
  } else
    return false;
}

void Parameters::CalculateParameters() {
  mMaximumSizeClassPopulations.resize(mNumberOfSizeClasses, 0);
  mRemainingVolumes.resize(mNumberOfSizeClasses);
  mLinearFeedingDenominators.resize(mNumberOfSizeClasses);
  mHalfSaturationConstants.resize(mNumberOfSizeClasses);
  mSizeClassMidPoints.resize(mNumberOfSizeClasses);
  mSizeClassBoundaries.resize(mNumberOfSizeClasses + 1);

  mSmallestVolumeExponent = std::log10(mSmallestIndividualVolume);
  mLargestVolumeExponent = std::log10(mLargestIndividualVolume);

  mTotalVolume = mInitialAutotrophicVolume + mInitialHeterotrophicVolume;

  double sizeClassExponentIncrement = (mLargestVolumeExponent - mSmallestVolumeExponent) / mNumberOfSizeClasses;

  for (unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex) {
    double sizeClassMidPointExponent = mSmallestVolumeExponent + ((sizeClassIndex + 0.5) * sizeClassExponentIncrement);
    double sizeClassBoundaryExponent = mSmallestVolumeExponent + (sizeClassIndex * sizeClassExponentIncrement);

    mSizeClassBoundaries[sizeClassIndex] = std::pow(10, sizeClassBoundaryExponent);
    mSizeClassMidPoints[sizeClassIndex] = std::pow(10, sizeClassMidPointExponent);

    mRemainingVolumes[sizeClassIndex] = mTotalVolume - mSizeClassMidPoints[sizeClassIndex];
    mLinearFeedingDenominators[sizeClassIndex] =
        (2 * mHalfSaturationConstantFraction) * mRemainingVolumes[sizeClassIndex];
    mHalfSaturationConstants[sizeClassIndex] = mHalfSaturationConstantFraction * mRemainingVolumes[sizeClassIndex];
    mMaximumSizeClassPopulations[sizeClassIndex] = std::ceil(mTotalVolume / mSizeClassMidPoints[sizeClassIndex]);
  }
  double sizeClassBoundaryExponent = mSmallestVolumeExponent + (mNumberOfSizeClasses * sizeClassExponentIncrement);
  mSizeClassBoundaries[mNumberOfSizeClasses] = std::pow(10, sizeClassBoundaryExponent);

  HeterotrophProcessor temporaryHeterotrophProcessor;
  mAutotrophSizeClassIndex = temporaryHeterotrophProcessor.FindSizeClassIndexFromVolume(mSmallestIndividualVolume);

  mInterSizeClassPreferenceMatrix.resize(mNumberOfSizeClasses);
  mInterSizeClassVolumeMatrix.resize(mNumberOfSizeClasses);

  for (unsigned subjectIndex = 0; subjectIndex < mNumberOfSizeClasses; ++subjectIndex) {
    double subjectVolumeMean = mSizeClassMidPoints[subjectIndex];
    double preferenceSum = 0;

    for (unsigned referenceIndex = 0; referenceIndex < mNumberOfSizeClasses; ++referenceIndex) {
      double referenceVolumeMean = mSizeClassMidPoints[referenceIndex];

      double preferenceForReferenceSizeClass =
          temporaryHeterotrophProcessor.CalculatePreferenceForPrey(subjectVolumeMean, referenceVolumeMean);

      preferenceSum += preferenceForReferenceSizeClass;

      mInterSizeClassPreferenceMatrix[subjectIndex].push_back(preferenceForReferenceSizeClass);
      mInterSizeClassVolumeMatrix[subjectIndex].push_back(preferenceForReferenceSizeClass * referenceVolumeMean);
    }
  }
}

bool Parameters::IsInitialised() {
  bool isInitialised = true;
  for (unsigned i = 0; i < Constants::eMutationStandardDeviation + 1; ++i)
    if (mParametersInitialised[i] == false) isInitialised = false;

  return isInitialised;
}

unsigned& Parameters::GetRunTimeInSeconds() { return mRunTimeInSeconds; }

unsigned& Parameters::GetRandomSeed() { return mRandomSeed; }

unsigned& Parameters::GetSamplingRate() { return mSamplingRate; }

unsigned& Parameters::GetNumberOfSizeClasses() { return mNumberOfSizeClasses; }

bool Parameters::GetReadModelState() { return mReadModelState; }

bool Parameters::GetWriteModelState() { return mWriteModelState; }

bool Parameters::GetUseLinearFeeding() { return mUseLinearFeeding; }

double& Parameters::GetInitialAutotrophVolume() { return mInitialAutotrophicVolume; }

double& Parameters::GetInitialHeterotrophVolume() { return mInitialHeterotrophicVolume; }

double& Parameters::GetMinimumHeterotrophicVolume() { return mMinimumHeterotrophicVolume; }

double& Parameters::GetSmallestIndividualVolume() { return mSmallestIndividualVolume; }

double& Parameters::GetLargestIndividualVolume() { return mLargestIndividualVolume; }

unsigned& Parameters::GetPreferredPreyVolumeRatio() { return mPreferredPreyVolumeRatio; }

double& Parameters::GetPreferenceFunctionWidth() { return mPreferenceFunctionWidth; }

double& Parameters::GetSizeClassSubsetFraction() { return mSizeClassSubsetFraction; }

double& Parameters::GetHalfSaturationConstantFraction() { return mHalfSaturationConstantFraction; }

double& Parameters::GetAssimilationEfficiency() { return mAssimilationEfficiency; }

double& Parameters::GetFractionalMetabolicExpense() { return mFractionalMetabolicExpense; }

double& Parameters::GetMetabolicIndex() { return mMetabolicIndex; }

double& Parameters::GetMutationProbability() { return mMutationProbability; }

double& Parameters::GetMutationStandardDeviation() { return mMutationStandardDeviation; }

unsigned& Parameters::GetAutotrophSizeClassIndex() { return mAutotrophSizeClassIndex; }

double& Parameters::GetSmallestVolumeExponent() { return mSmallestVolumeExponent; }

double& Parameters::GetLargestVolumeExponent() { return mLargestVolumeExponent; }

double& Parameters::GetSizeClassBoundary(const unsigned index) { return mSizeClassBoundaries[index]; }

double& Parameters::GetSizeClassMidPoint(const unsigned index) { return mSizeClassMidPoints[index]; }

const std::vector<double>& Parameters::GetSizeClassBoundaries() { return mSizeClassBoundaries; }

const std::vector<double>& Parameters::GetSizeClassMidPoints() { return mSizeClassMidPoints; }

const std::vector<double>& Parameters::GetLinearFeedingDenominators() { return mLinearFeedingDenominators; }

const std::vector<double>& Parameters::GetHalfSaturationConstants() { return mHalfSaturationConstants; }

const std::vector<unsigned>& Parameters::GetMaximumSizeClassPopulations() { return mMaximumSizeClassPopulations; }

double& Parameters::GetInterSizeClassPreference(const unsigned predatorIndex, const unsigned preyIndex) {
  return mInterSizeClassPreferenceMatrix[predatorIndex][preyIndex];
}

double& Parameters::GetInterSizeClassVolume(const unsigned predatorIndex, const unsigned preyIndex) {
  return mInterSizeClassVolumeMatrix[predatorIndex][preyIndex];
}

double& Parameters::GetTotalVolume() { return mTotalVolume; }

unsigned& Parameters::GetMaximumSizeClassPopulation(const unsigned sizeClassIndex) {
  return mMaximumSizeClassPopulations[sizeClassIndex];
}

double& Parameters::GetRemainingVolume(const unsigned sizeClassIndex) { return mRemainingVolumes[sizeClassIndex]; }

double& Parameters::GetLinearFeedingDenominator(const unsigned sizeClassIndex) {
  return mLinearFeedingDenominators[sizeClassIndex];
}

double& Parameters::GetHalfSaturationConstant(const unsigned sizeClassIndex) {
  return mHalfSaturationConstants[sizeClassIndex];
}

const std::vector<double>& Parameters::GetInterSizeClassPreferenceVector(const unsigned index) const {
  return mInterSizeClassPreferenceMatrix[index];
}

const std::vector<double>& Parameters::GetInterSizeClassVolumeVector(const unsigned index) const {
  return mInterSizeClassVolumeMatrix[index];
}

const std::vector<std::vector<double>>& Parameters::GetInterSizeClassPreferenceMatrix() const {
  return mInterSizeClassPreferenceMatrix;
}

const std::vector<std::vector<double>>& Parameters::GetInterSizeClassVolumeMatrix() const { return mInterSizeClassVolumeMatrix; }

void Parameters::SetRandomSeed(const unsigned randomNumberSeed) {
  mRandomSeed = randomNumberSeed;
  mParametersInitialised[Constants::eRandomSeed] = true;
}

void Parameters::SetRunTimeInSeconds(const unsigned runTimeInSeconds) {
  mRunTimeInSeconds = runTimeInSeconds;
  mParametersInitialised[Constants::eRunTimeInSeconds] = true;
}

void Parameters::SetSamplingRate(const unsigned samplingRate) {
  mSamplingRate = samplingRate;
  mParametersInitialised[Constants::eSamplingRate] = true;
}

void Parameters::SetNumberOfSizeClasses(const unsigned numberOfSizeClasses) {
  mNumberOfSizeClasses = numberOfSizeClasses;
  mParametersInitialised[Constants::eNumberOfSizeClasses] = true;
}

void Parameters::SetReadModelState(const bool createNewPopulation) {
  mReadModelState = createNewPopulation;
  mParametersInitialised[Constants::eReadModelState] = true;
}

void Parameters::SetWriteModelState(const bool writeModelState) {
  mWriteModelState = writeModelState;
  mParametersInitialised[Constants::eWriteModelState] = true;
}

void Parameters::SetUseLinearFeeding(const bool useLinearFeeding) {
  mUseLinearFeeding = useLinearFeeding;
  mParametersInitialised[Constants::eUseLinearFeeding] = true;
}

void Parameters::SetInitialAutotrophicVolume(const double initialAutotrophicVolume) {
  mInitialAutotrophicVolume = initialAutotrophicVolume;
  mParametersInitialised[Constants::eInitialAutotrophicVolume] = true;
}

void Parameters::SetInitialHeterotrophicVolume(const double initialHeterotrophicVolume) {
  mInitialHeterotrophicVolume = initialHeterotrophicVolume;
  mParametersInitialised[Constants::eInitialHeterotrophicVolume] = true;
}

void Parameters::SetMinimumHeterotrophicVolume(const double minimumHeterotrophicVolume) {
  mMinimumHeterotrophicVolume = minimumHeterotrophicVolume;
  mParametersInitialised[Constants::eMinimumHeterotrophicVolume] = true;
}

void Parameters::SetSmallestIndividualVolume(double smallestIndividualVolume) {
  mSmallestIndividualVolume = smallestIndividualVolume;
  mParametersInitialised[Constants::eSmallestIndividualVolume] = true;
}

void Parameters::SetLargestIndividualVolume(double largestIndividualVolume) {
  mLargestIndividualVolume = largestIndividualVolume;
  mParametersInitialised[Constants::eLargestIndividualVolume] = true;
}

void Parameters::SetPreferredPreyVolumeRatio(const unsigned preferredPreyVolumeRatio) {
  mPreferredPreyVolumeRatio = preferredPreyVolumeRatio;
  mParametersInitialised[Constants::ePreferredPreyVolumeRatio] = true;
}

void Parameters::SetPreferenceFunctionWidth(const double preferenceFunctionWidth) {
  mPreferenceFunctionWidth = preferenceFunctionWidth;
  mParametersInitialised[Constants::ePreferenceFunctionWidth] = true;
}

void Parameters::SetSizeClassSubsetFraction(const double sizeClassSubsetFraction) {
  mSizeClassSubsetFraction = sizeClassSubsetFraction;
  mParametersInitialised[Constants::eSizeClassSubsetFraction] = true;
}

void Parameters::SetHalfSaturationConstantFraction(const double halfSaturationConstantFraction) {
  mHalfSaturationConstantFraction = halfSaturationConstantFraction;
  mParametersInitialised[Constants::eHalfSaturationConstantFraction] = true;
}

void Parameters::SetAssimilationEfficiency(const double assimilationEfficiency) {
  mAssimilationEfficiency = assimilationEfficiency;
  mParametersInitialised[Constants::eAssimilationEfficiency] = true;
}

void Parameters::SetFractionalMetabolicExpense(const double fractionalMetabolicExpense) {
  mFractionalMetabolicExpense = fractionalMetabolicExpense;
  mParametersInitialised[Constants::eFractionalMetabolicExpense] = true;
}

void Parameters::SetMetabolicIndex(const double metabolicIndex) {
  mMetabolicIndex = metabolicIndex;
  mParametersInitialised[Constants::eMetabolicIndex] = true;
}

void Parameters::SetMutationProbability(const double mutationProbability) {
  mMutationProbability = mutationProbability;
  mParametersInitialised[Constants::eMutationProbability] = true;
}

void Parameters::SetMutationStandardDeviation(const double mutationStandardDeviation) {
  mMutationStandardDeviation = mutationStandardDeviation;
  mParametersInitialised[Constants::eMutationStandardDeviation] = true;
}
