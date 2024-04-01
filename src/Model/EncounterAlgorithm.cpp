/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "EncounterAlgorithm.h"

#include <iterator>

#include "Parameters.h"

EncounterAlgorithm::EncounterAlgorithm(EcologicalData& data, EcologicalFunctions& functions,
                                       Parameters& params, const unsigned& randomSeed):
    data_(data),
    functions_(functions),
    params_(params),
    random_(randomSeed),
    interSizeClassPreferences_(data_.getInterSizeClassPreferences()),
    interSizeClassVolumes_(data_.getInterSizeClassVolumes()),
    numberOfSizeClasses_(params_.getNumberOfSizeClasses()) {
}

void EncounterAlgorithm::update(std::vector<SizeClass>& sizeClasses, SizeClass& sizeClass) {
  std::vector<SizeClass>::iterator coupledSizeClassIt = calcFeedingProbability(sizeClasses, sizeClass);
  sizeClass.sizeClassSubset([&](unsigned randomIndex) {
    Heterotroph& predator = sizeClass.getHeterotroph(randomIndex);

    if (feedingStrategy_ == enums::eCarnivore) {
      feedFromHeterotrophs(predator, coupledSizeClassIt);
    } else if (feedingStrategy_ == enums::eHerbivore){
      feedFromAutotrophs(predator);
    }
  });
}

std::vector<SizeClass>::iterator EncounterAlgorithm::calcFeedingProbability(std::vector<SizeClass>& sizeClasses,
                                                                            SizeClass& sizeClass) {
  std::vector<SizeClass>::iterator coupledSizeClassIt = sizeClasses.begin();
  if (sizeClass.getPopulationSize() != 0) {
    std::vector<double> effectiveSizeClassVolumes(numberOfSizeClasses_, 0);
    calcEffectiveSizeClassVolumes(sizeClasses, sizeClass, effectiveSizeClassVolumes);
    coupledSizeClassIt = setCoupledSizeClass(effectiveSizeClassVolumes, sizeClasses);
    calcFeedingStrategy();
    feedingProbabilty_ = functions_.functionalResponseNonLinear(sizeClass.getIndex(), effectivePreyVolume_);  // PJU FIX - Introduce option switch?
  }
  return coupledSizeClassIt;
}

void EncounterAlgorithm::calcEffectiveSizeClassVolumes(std::vector<SizeClass>& sizeClasses, SizeClass& thisSizeClass,
                                                       std::vector<double>& effectiveSizeClassVolumes) {
  effectivePreyVolume_ = 0;
  effectiveAutotrophVolume_ = 0;
  feedingProbabilty_ = 0;

  // PJU FIX - These may be modified when work is complete on the Parameters class.
  auto sizeClassVolumesIt = interSizeClassVolumes_[thisSizeClass.getIndex()].begin();
  auto sizeClassPreferencesIt = interSizeClassPreferences_[thisSizeClass.getIndex()].begin();

  std::vector<double>::iterator effectiveSizeClassVolumesIt = effectiveSizeClassVolumes.begin();

  std::for_each(std::begin(sizeClasses), std::end(sizeClasses), [&](SizeClass& otherSizeClass) {
    std::size_t populationSize = otherSizeClass.getPopulationSize();
    if (&thisSizeClass == &otherSizeClass) {
      populationSize--;  // Reduce population size for a single individual for this size class.
    }
    *effectiveSizeClassVolumesIt = *sizeClassVolumesIt * populationSize;
    effectiveAutotrophVolume_ = *sizeClassPreferencesIt * otherSizeClass.getAutotrophs().getVolume();

    *effectiveSizeClassVolumesIt += effectiveAutotrophVolume_;
    effectivePreyVolume_ += *effectiveSizeClassVolumesIt;

    std::advance(effectiveSizeClassVolumesIt, 1);
    std::advance(sizeClassPreferencesIt, 1);
    std::advance(sizeClassVolumesIt, 1);
  });
}

std::vector<SizeClass>::iterator EncounterAlgorithm::setCoupledSizeClass(
                                                                const std::vector<double>& effectiveSizeClassVolumes,
                                                                std::vector<SizeClass>& sizeClasses) {
  // Default to largest populated size class to prevent un-earned predation from taking place
  std::vector<SizeClass>::iterator coupledSizeClassIt = std::prev(sizeClasses.end());
  // Add noise to the threshold to permit a mixed strategy (mixotroph)
  double randEffectivePreyValue = random_.getUniform() * effectivePreyVolume_;
  double effectivePreySum = 0;
  int index = 0;
  // Return from find_if is not used - used here to mimic break statement in classic for
  std::find_if (begin(effectiveSizeClassVolumes), end(effectiveSizeClassVolumes),
                                                  [&](double effectiveSizeClassVolume) {
    effectivePreySum += effectiveSizeClassVolume;
    if (effectivePreySum >= randEffectivePreyValue) {
      int sizeClassIndex = -numberOfSizeClasses_ + index;  // Calculate appropriate index reduction from end
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
  if (effectiveAutotrophVolume_ > 0) {
    double probHerbivory = effectiveAutotrophVolume_ / effectivePreyVolume_;
    if (random_.getUniform() <= probHerbivory) {
      feedingStrategy_ = enums::eHerbivore;
    }
  }
}

void EncounterAlgorithm::feedFromHeterotrophs(Heterotroph& predator, std::vector<SizeClass>::iterator coupledSizeClassIt) {
    if (coupledSizeClassIt->getPopulationSize() != 0) {
      Heterotroph& prey = coupledSizeClassIt->getRandomHeterotroph();
      while(&predator == &prey) { // Predators cannot eat themselves
        prey = coupledSizeClassIt->getRandomHeterotroph();
      }
    }
//  Types::HeterotrophPointer prey = GetRandomPreyFromSizeClass(coupledIndex, predator);
//  if (prey != nullptr) {
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
//  if (mAutotrophs.GetVolume( ) > mSmallestIndividualVolume) {
//    mAutotrophs.SubtractFromVolume(mSmallestIndividualVolume);
//    mHeterotrophData.IncrementVegetarianFrequencies(grazer);

//    double waste = grazer->ConsumePreyVolume(mSmallestIndividualVolume);
//    ++mFedCount[grazer->GetSizeClassIndex()];

//    mHeterotrophProcessor.UpdateHerbivoreTrophicIndex(grazer);
//    mNutrient.AddToVolume(waste);
//  }
}
