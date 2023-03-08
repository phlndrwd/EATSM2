#include "HeterotrophData.h"

#include "DataRecorder.h"
#include "Heterotroph.h"
#include "Parameters.h"

HeterotrophData::HeterotrophData() :
//      sizeClassMidPointsFloat_(Parameters::Get()->getSizeClassMidPoints().begin(),
//                               Parameters::Get()->getSizeClassMidPoints().end()),
//      sizeClassBoundariesFloat_(Parameters::Get()->getSizeClassBoundaries().begin(),
//                                Parameters::Get()->getSizeClassBoundaries().end()),
      minimumHeterotrophicVolume_(Parameters::Get()->getMinimumHeterotrophicVolume()),
      smallestIndividualVolume_(Parameters::Get()->getSmallestIndividualVolume()),
      numberOfSizeClasses_(Parameters::Get()->getNumberOfSizeClasses()) {
  // Convert double vectors used in the model to floats for writing to file
  DataRecorder::get()->setVectorDataOn("AxisSizeClassMidPointValues", sizeClassMidPointsFloat_);
  DataRecorder::get()->setVectorDataOn("AxisSizeClassBoundaryValues", sizeClassBoundariesFloat_);

  effectiveSizeClassVolumeMatrix_.resize(numberOfSizeClasses_);
  for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
    effectiveSizeClassVolumeMatrix_[sizeClassIndex].resize(numberOfSizeClasses_, 0);
  }
  sizeClassCouplings_.resize(numberOfSizeClasses_, constants::kMissingValue);
  sizeClassEffectivePreyVolumes_.resize(numberOfSizeClasses_, 0);
  sizeClassFeedingProbabilities_.resize(numberOfSizeClasses_, 0);
  sizeClassHerbivoreFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassCarnivoreFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassPreyFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassPreyVolumeRatios_.resize(numberOfSizeClasses_, 0);
  sizeClassParentFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassChildFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassVolumeMutantFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassStarvedFrequencies_.resize(numberOfSizeClasses_, 0);

  inFlux_ = 0;
  toFlux_ = 0;
  volume_ = 0;
  approxVolume_ = 0;
  frequency_ = 0;
}

HeterotrophData::~HeterotrophData() {}

void HeterotrophData::initialiseDataStructures() {
  sizeClassPopulation_.clear();
  sizeClassVolumes_.clear();
  sizeClassApproxVolumes_.clear();
  sizeClassGrowthRatios_.clear();
  sizeClassTrophicClassifications_.clear();
  sizeClassAges_.clear();

  sizeClassPopulation_.resize(numberOfSizeClasses_, constants::kMissingValue);
  sizeClassVolumes_.resize(numberOfSizeClasses_, 0);
  sizeClassApproxVolumes_.resize(numberOfSizeClasses_, constants::kMissingValue);
  sizeClassGrowthRatios_.resize(numberOfSizeClasses_, 0);
  sizeClassTrophicClassifications_.resize(numberOfSizeClasses_, 0);
  sizeClassAges_.resize(numberOfSizeClasses_, 0);

  trophicFrequencies_.clear();
  trophicVolumes_.clear();
  trophicAges_.clear();

  trophicFrequencies_.resize(constants::kMaximumNumberOfTrophicLevels,
                             0);  // 0 = unclassified, 1 = primary producer, 2 = secondary producer
  trophicVolumes_.resize(constants::kMaximumNumberOfTrophicLevels, 0);
  trophicAges_.resize(constants::kMaximumNumberOfTrophicLevels, 0);

  volume_ = 0;
  approxVolume_ = 0;
  frequency_ = 0;
}

void HeterotrophData::recordOutputData() {
  DataRecorder::get()->addDataTo("HeterotrophFrequency", frequency_);
  DataRecorder::get()->addDataTo("HeterotrophVolume", volume_);
  DataRecorder::get()->addDataTo("HeterotrophApproxVolume", approxVolume_);
  DataRecorder::get()->addDataTo("ToHeterotrophFlux", toFlux_);
  DataRecorder::get()->addDataTo("InHeterotrophFlux", inFlux_);

  DataRecorder::get()->addDataTo("TimingFeeding", timeFeeding_);
  DataRecorder::get()->addDataTo("TimingMetabolising", timeMetabolising_);
  DataRecorder::get()->addDataTo("TimingReproducing", timeReproducing_);
  DataRecorder::get()->addDataTo("TimingStarving", timeStarving_);

  DataRecorder::get()->addDataTo("SizeClassPopulation", sizeClassPopulation_);
  DataRecorder::get()->addDataTo("SizeClassHerbivoreFrequencies", sizeClassHerbivoreFrequencies_);
  DataRecorder::get()->addDataTo("SizeClassCarnivoreFrequencies", sizeClassCarnivoreFrequencies_);
  DataRecorder::get()->addDataTo("SizeClassPreyFrequencies", sizeClassPreyFrequencies_);
  DataRecorder::get()->addDataTo("SizeClassStarvedFrequencies", sizeClassStarvedFrequencies_);
  DataRecorder::get()->addDataTo("SizeClassParentFrequencies", sizeClassParentFrequencies_);
  DataRecorder::get()->addDataTo("SizeClassChildFrequencies", sizeClassChildFrequencies_);
  DataRecorder::get()->addDataTo("SizeClassVolumeMutantFrequencies", sizeClassVolumeMutantFrequencies_);
  DataRecorder::get()->addDataTo("SizeClassVolumes", sizeClassVolumes_);
  DataRecorder::get()->addDataTo("SizeClassApproxVolumes", sizeClassApproxVolumes_);
  DataRecorder::get()->addDataTo("SizeClassEffectivePreyVolumes", sizeClassEffectivePreyVolumes_);
  DataRecorder::get()->addDataTo("SizeClassGrowthRatios", sizeClassGrowthRatios_);
  DataRecorder::get()->addDataTo("SizeClassCouplings", sizeClassCouplings_);
  DataRecorder::get()->addDataTo("SizeClassPreyVolumeRatios", sizeClassPreyVolumeRatios_);
  DataRecorder::get()->addDataTo("SizeClassFeedingProbabilities", sizeClassFeedingProbabilities_);
  DataRecorder::get()->addDataTo("SizeClassTrophicClassifications", sizeClassTrophicClassifications_);
  DataRecorder::get()->addDataTo("SizeClassAges", sizeClassAges_);

  DataRecorder::get()->addDataTo("TrophicFrequencies", trophicFrequencies_);
  DataRecorder::get()->addDataTo("TrophicVolumes", trophicVolumes_);
  DataRecorder::get()->addDataTo("TrophicAges", trophicAges_);

  resetDataStructures();
}

double HeterotrophData::getEffectiveSizeClassVolume(const unsigned predatorIndex, const unsigned preyIndex) const {
  return effectiveSizeClassVolumeMatrix_[predatorIndex][preyIndex];
}

double HeterotrophData::getEffectivePreyVolume(const unsigned sizeClassIndex) {
  return sizeClassEffectivePreyVolumes_[sizeClassIndex];
}

double HeterotrophData::getFeedingProbability(const unsigned sizeClassIndex) {
  return sizeClassFeedingProbabilities_[sizeClassIndex];
}

unsigned HeterotrophData::getCoupledSizeClassIndex(const unsigned predatorIndex) {
  return sizeClassCouplings_[predatorIndex];
}

void HeterotrophData::setEffectiveSizeClassVolume(const unsigned predatorIndex, const unsigned preyIndex,
                                                  const double effectiveSizeClassVolume) {
  effectiveSizeClassVolumeMatrix_[predatorIndex][preyIndex] = effectiveSizeClassVolume;
}

void HeterotrophData::setEffectivePreyVolume(const unsigned predatorIndex, const double effectivePreyVolume) {
  sizeClassEffectivePreyVolumes_[predatorIndex] = effectivePreyVolume;
}

void HeterotrophData::setFeedingProbability(const unsigned sizeClassIndex, const double feedingProbability) {
  sizeClassFeedingProbabilities_[sizeClassIndex] = feedingProbability;
}

void HeterotrophData::setCoupledSizeClassIndex(const unsigned sizeClassIndex, const unsigned coupledIndex) {
  sizeClassCouplings_[sizeClassIndex] = coupledIndex;
}

void HeterotrophData::addIndividualData(const Heterotroph* individual) {
  addTrophicLevel(individual->getTrophicLevel(), individual->getVolumeActual(), individual->getSizeClassIndex(),
                  individual->getAge());
  sizeClassGrowthRatios_[individual->getSizeClassIndex()] +=
      (individual->getVolumeActual() / individual->getVolumeHeritable());
  sizeClassVolumes_[individual->getSizeClassIndex()] += individual->getVolumeActual();
  volume_ += individual->getVolumeActual();
  sizeClassAges_[individual->getSizeClassIndex()] += individual->getAge();
}

void HeterotrophData::addSizeClassData(const unsigned sizeClassIndex, const unsigned sizeClassPopulation,
                                       const double sizeClassMultiplier) {
  frequency_ += sizeClassPopulation;

  double sizeClassVolumeApproximation = sizeClassMidPointsFloat_[sizeClassIndex] * sizeClassPopulation;
  approxVolume_ += sizeClassVolumeApproximation;
  if (sizeClassPopulation > 0) {
    sizeClassPopulation_[sizeClassIndex] = sizeClassPopulation;
    sizeClassApproxVolumes_[sizeClassIndex] = sizeClassVolumeApproximation;
    sizeClassGrowthRatios_[sizeClassIndex] = sizeClassGrowthRatios_[sizeClassIndex] * sizeClassMultiplier;
    sizeClassTrophicClassifications_[sizeClassIndex] =
        sizeClassTrophicClassifications_[sizeClassIndex] * sizeClassMultiplier;
    sizeClassAges_[sizeClassIndex] = sizeClassAges_[sizeClassIndex] * sizeClassMultiplier;
  } else {
    sizeClassVolumes_[sizeClassIndex] = constants::kMissingValue;
    sizeClassGrowthRatios_[sizeClassIndex] = constants::kMissingValue;
    sizeClassTrophicClassifications_[sizeClassIndex] = constants::kMissingValue;
    sizeClassAges_[sizeClassIndex] = constants::kMissingValue;
  }
}

void HeterotrophData::addTrophicLevel(const double trophicLevel, const double volumeActual,
                                      const unsigned sizeClassIndex, const unsigned age) {
  for (unsigned trophicIndex = 0; trophicIndex <= constants::kMaximumNumberOfTrophicLevels; ++trophicIndex) {
    if (trophicLevel < (trophicIndex + 0.5)) {
      unsigned discreteTrophicLevel = trophicIndex;
      ++trophicFrequencies_[discreteTrophicLevel];
      trophicVolumes_[discreteTrophicLevel] += volumeActual;
      trophicAges_[discreteTrophicLevel] += age;

      break;
    }
  }
  sizeClassTrophicClassifications_[sizeClassIndex] += trophicLevel;
}

void HeterotrophData::normaliseData() {
  for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
    if (sizeClassPreyVolumeRatios_[sizeClassIndex] > 0) {
      sizeClassPreyVolumeRatios_[sizeClassIndex] =
          sizeClassPreyVolumeRatios_[sizeClassIndex] /
          (sizeClassCarnivoreFrequencies_[sizeClassIndex] + sizeClassHerbivoreFrequencies_[sizeClassIndex]);
    } else
      sizeClassPreyVolumeRatios_[sizeClassIndex] = constants::kMissingValue;
  }
  for (unsigned trophicIndex = 0; trophicIndex < constants::kMaximumNumberOfTrophicLevels; ++trophicIndex) {
    if (trophicFrequencies_[trophicIndex] > 0)
      trophicAges_[trophicIndex] = trophicAges_[trophicIndex] / (double)trophicFrequencies_[trophicIndex];
  }
}

bool HeterotrophData::areHeterotrophsAlive() const { return volume_ > minimumHeterotrophicVolume_; }

void HeterotrophData::resetDataStructures() {
  toFlux_ = 0;
  inFlux_ = 0;

  timeFeeding_ = 0;
  timeMetabolising_ = 0;
  timeReproducing_ = 0;
  timeStarving_ = 0;

  sizeClassHerbivoreFrequencies_.clear();
  sizeClassPreyFrequencies_.clear();
  sizeClassCarnivoreFrequencies_.clear();
  sizeClassStarvedFrequencies_.clear();
  sizeClassParentFrequencies_.clear();
  sizeClassChildFrequencies_.clear();
  sizeClassVolumeMutantFrequencies_.clear();

  sizeClassPreyVolumeRatios_.clear();

  sizeClassEffectivePreyVolumes_.clear();
  sizeClassFeedingProbabilities_.clear();
  sizeClassCouplings_.clear();

  sizeClassHerbivoreFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassPreyFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassCarnivoreFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassStarvedFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassParentFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassChildFrequencies_.resize(numberOfSizeClasses_, 0);
  sizeClassVolumeMutantFrequencies_.resize(numberOfSizeClasses_, 0);

  sizeClassPreyVolumeRatios_.resize(numberOfSizeClasses_, 0);

  sizeClassEffectivePreyVolumes_.resize(numberOfSizeClasses_, constants::kMissingValue);
  sizeClassFeedingProbabilities_.resize(numberOfSizeClasses_, constants::kMissingValue);
  sizeClassCouplings_.resize(numberOfSizeClasses_, constants::kMissingValue);
}

void HeterotrophData::incrementVegetarianFrequencies(const Heterotroph* grazer) {
  ++sizeClassHerbivoreFrequencies_[grazer->getSizeClassIndex()];

  double preyVolumeRatio = grazer->getVolumeActual() / smallestIndividualVolume_;
  sizeClassPreyVolumeRatios_[grazer->getSizeClassIndex()] =
      sizeClassPreyVolumeRatios_[grazer->getSizeClassIndex()] + preyVolumeRatio;
  toFlux_ += smallestIndividualVolume_;
}

void HeterotrophData::incrementCarnivoreFrequencies(const Heterotroph* predator,
                                                    const Heterotroph* prey) {
  ++sizeClassCarnivoreFrequencies_[predator->getSizeClassIndex()];
  ++sizeClassPreyFrequencies_[prey->getSizeClassIndex()];

  double preyVolumeRatio = predator->getVolumeActual() / prey->getVolumeActual();
  sizeClassPreyVolumeRatios_[predator->getSizeClassIndex()] =
      sizeClassPreyVolumeRatios_[predator->getSizeClassIndex()] + preyVolumeRatio;
  inFlux_ += prey->getVolumeActual();
}

void HeterotrophData::incrementStarvedFrequencies(const unsigned sizeClassIndex) {
  ++sizeClassStarvedFrequencies_[sizeClassIndex];
}

void HeterotrophData::incrementMutantFrequency(const unsigned sizeClassIndex, const unsigned geneIndex) {
  if (geneIndex == constants::eVolume) ++sizeClassVolumeMutantFrequencies_[sizeClassIndex];
}

void HeterotrophData::incrementParentFrequencies(const unsigned sizeClassIndex) {
  ++sizeClassParentFrequencies_[sizeClassIndex];
}

void HeterotrophData::incrementChildFrequencies(const unsigned sizeClassIndex) {
  ++sizeClassChildFrequencies_[sizeClassIndex];
}

void HeterotrophData::addToTimeFeeding(const double timeFeeding) {
  timeFeeding_ += timeFeeding;
}

void HeterotrophData::addToTimeMetabolising(const double timeMetabolising) {
  timeMetabolising_ += timeMetabolising;
}

void HeterotrophData::addToTimeReproducing(const double timeReproducing) {
  timeReproducing_ += timeReproducing;
}

void HeterotrophData::addToTimeStarving(const double timeStarving) {
  timeStarving_ += timeStarving;
}
