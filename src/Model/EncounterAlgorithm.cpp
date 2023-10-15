#include "EncounterAlgorithm.h"

#include "Parameters.h"

EncounterAlgorithm::EncounterAlgorithm(const unsigned randomSeed) :
    random_(randomSeed),
    interSizeClassPreferences_(Parameters::Get()->getInterSizeClassPreferenceMatrix()),
    interSizeClassVolumes_(Parameters::Get()->getInterSizeClassVolumeMatrix()),
    linearFeedingDenominators_(Parameters::Get()->getLinearFeedingDenominators()),
    halfSaturationConstants_(Parameters::Get()->getHalfSaturationConstants()),
    numberOfSizeClasses_(Parameters::Get()->getNumberOfSizeClasses()){}

void EncounterAlgorithm::update(std::vector<SizeClass>& sizeClasses,
                                SizeClass& sizeClass) {
  std::vector<SizeClass>::iterator coupledSizeClassIt = calcFeedingProbability(sizeClasses, sizeClass);
  sizeClass.sizeClassSubset([&](unsigned randomIndex) {
    Heterotroph& predator = sizeClass.getHeterotroph(randomIndex);

    if(feedingStrategy_ == enums::eCarnivore) {
      feedFromHeterotrophs(predator, coupledSizeClassIt);
    } else if(feedingStrategy_ == enums::eHerbivore){
      feedFromAutotrophs(predator);
    }
  });
}

std::vector<SizeClass>::iterator EncounterAlgorithm::calcFeedingProbability(std::vector<SizeClass>& sizeClasses,
                                                                            SizeClass& sizeClass) {
  std::vector<SizeClass>::iterator coupledSizeClassIt = sizeClasses.begin();
  if(sizeClass.getPopulationSize() != 0) {
    std::vector<double> effectiveSizeClassVolumes(numberOfSizeClasses_, 0);
    calcEffectiveSizeClassVolumes(sizeClasses, sizeClass, effectiveSizeClassVolumes);
    coupledSizeClassIt = setCoupledSizeClass(effectiveSizeClassVolumes, sizeClasses);
    calcFeedingStrategy();
    feedingProbabilty_ = functionalResponseNonLinear(sizeClass.getIndex(), effectivePreyVolume_);  // Introduce option switch?
  }
  return coupledSizeClassIt;
}


void EncounterAlgorithm::calcEffectiveSizeClassVolumes(std::vector<SizeClass>& sizeClasses,
                                                       SizeClass& sizeClass,
                                                       std::vector<double>& effectiveSizeClassVolumes) {
  effectivePreyVolume_ = 0;
  effectiveAutotrophVolume_ = 0;
  feedingProbabilty_ = 0;

  std::vector<size_t> populationSizes = getPopulationSizes(sizeClasses, sizeClass);

  auto sizeClassVolumesIt = interSizeClassVolumes_[sizeClass.getIndex()].begin();
  auto sizeClassPreferencesIt = interSizeClassPreferences_[sizeClass.getIndex()].begin();
  std::vector<double>::iterator effectiveSizeClassVolumesIt = effectiveSizeClassVolumes.begin();
  // Calculate effective prey volumes
  std::for_each(begin(populationSizes), end(populationSizes),
  [&](size_t populationSize) {

    *effectiveSizeClassVolumesIt = *sizeClassVolumesIt * populationSize;
    effectiveAutotrophVolume_ = *sizeClassPreferencesIt * sizeClass.getAutotrophs().getVolume();

    *effectiveSizeClassVolumesIt += effectiveAutotrophVolume_;
    effectivePreyVolume_ += *effectiveSizeClassVolumesIt;

    std::next(effectiveSizeClassVolumesIt);
    std::next(sizeClassPreferencesIt);
    std::next(sizeClassVolumesIt);
//    std::advance(effectiveSizeClassVolumesIt, 1);
//    std::advance(sizeClassPreferencesIt, 1);
//    std::advance(sizeClassVolumesIt, 1);
  });
}

std::vector<size_t> EncounterAlgorithm::getPopulationSizes(std::vector<SizeClass>& sizeClasses, SizeClass& sizeClass) {
  std::vector<size_t> populationSizes(numberOfSizeClasses_, 0);

  auto popSizesIt = populationSizes.begin();
  std::for_each(std::begin(sizeClasses), std::end(sizeClasses),
  [&](SizeClass& otherSizeClass) {
    size_t popSize = otherSizeClass.getPopulationSize();
    if(&otherSizeClass == &sizeClass) {
      popSize--;
    }
    *popSizesIt = popSize;
    std::advance(popSizesIt, 1);
  });
  return populationSizes;
}

std::vector<SizeClass>::iterator EncounterAlgorithm::setCoupledSizeClass(const std::vector<double>& effectiveSizeClassVolumes,
                                                                std::vector<SizeClass>& sizeClasses) {
  // Default to largest populated size class
  std::vector<SizeClass>::iterator coupledSizeClassIt = std::prev(sizeClasses.end());
  double randEffectivePreyValue = random_.getUniform() * effectivePreyVolume_;
  double effectivePreySum = 0;
  unsigned index = 0;
  // Return from find_if is not used - used here to mimic break statement in classic for
  std::find_if(begin(effectiveSizeClassVolumes), end(effectiveSizeClassVolumes),
    [&](double effectiveSizeClassVolume) {
    effectivePreySum += effectiveSizeClassVolume;
    if(effectivePreySum >= randEffectivePreyValue) {
      int sizeClassIndex = -(numberOfSizeClasses_ - 1) + index;
      std::advance(coupledSizeClassIt, sizeClassIndex);
      return true;
    } else {
      ++index;
      return false;
    }
  });
  return coupledSizeClassIt;
}

void EncounterAlgorithm::calcFeedingStrategy() {
  feedingStrategy_ = enums::eCarnivore;
  double probHerbivory = effectiveAutotrophVolume_ / effectivePreyVolume_;
  if(random_.getUniform() <= probHerbivory) {
    feedingStrategy_ = enums::eHerbivore;
  }
}

double EncounterAlgorithm::functionalResponseLinear(const unsigned predatorIndex,
                                                          const double effectivePreyVolume) {
  return std::min(effectivePreyVolume / linearFeedingDenominators_[predatorIndex], 1.0);
}

double EncounterAlgorithm::functionalResponseNonLinear(const unsigned predatorIndex,
                                                       const double effectivePreyVolume) {
  return (effectivePreyVolume / (halfSaturationConstants_[predatorIndex] + effectivePreyVolume));
}

void EncounterAlgorithm::feedFromHeterotrophs(Heterotroph& predator, std::vector<SizeClass>::iterator coupledSizeClassIt) {
    if(coupledSizeClassIt->getPopulationSize() != 0) {
      Heterotroph& prey = coupledSizeClassIt->getRandomHeterotroph();
      while(&predator == &prey) { // Predators cannot eat themselves
        prey = coupledSizeClassIt->getRandomHeterotroph();
      }
    }
//  Types::HeterotrophPointer prey = GetRandomPreyFromSizeClass(coupledIndex, predator);
//  if(prey != nullptr) {
//    double preyVolume = prey->GetVolumeActual();
//    mHeterotrophData.IncrementCarnivoreFrequencies(predator, prey);

//    double waste = predator->ConsumePreyVolume(preyVolume);
//    ++mFedCount[predator->GetSizeClassIndex()];

//    mHeterotrophProcessor.UpdateCarnivoreTrophicIndex(predator, prey);
//    mNutrient.AddToVolume(waste);
//    Kill(prey);
//  }
}

void EncounterAlgorithm::feedFromAutotrophs(Heterotroph& grazer) {
//  if(mAutotrophs.GetVolume( ) > mSmallestIndividualVolume) {
//    mAutotrophs.SubtractFromVolume(mSmallestIndividualVolume);
//    mHeterotrophData.IncrementVegetarianFrequencies(grazer);

//    double waste = grazer->ConsumePreyVolume(mSmallestIndividualVolume);
//    ++mFedCount[grazer->GetSizeClassIndex()];

//    mHeterotrophProcessor.UpdateHerbivoreTrophicIndex(grazer);
//    mNutrient.AddToVolume(waste);
//  }
}
