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

#include "Constants.h"
#include "Parameters.h"

EncounterAlgorithm::EncounterAlgorithm(Nutrient& nutrient, EcologicalData& data, Parameters& params,
                                       const std::uint32_t& randomSeed):
    nutrient_(nutrient),
    data_(data),
    functions_(data_, params),
    random_(randomSeed),
    interSizeClassPreferences_(data_.getInterSizeClassPreferences()),
    interSizeClassVolumes_(data_.getInterSizeClassVolumes()),
    numberOfSizeClasses_(params.getNumberOfSizeClasses()),
    autotrophCellSize_(data.getAutotrophCellSize()) {}

void EncounterAlgorithm::update(std::vector<SizeClass>& sizeClasses, SizeClass& thisSizeClass) {
  enums::eFeedingStrategy feedingStrategy = enums::eNotEating;
  std::vector<SizeClass>::iterator coupledSizeClassIt = sizeClasses.begin();
  double feedingProbability = calcFeedingProbability(sizeClasses, thisSizeClass, coupledSizeClassIt, feedingStrategy);

  thisSizeClass.getHeterotrophs().subset([&](std::uint32_t randomIndex) {
    if (random_.getUniform() <= feedingProbability) {
      Heterotroph& predator = thisSizeClass.getHeterotrophs().getHeterotroph(randomIndex);
      if (feedingStrategy == enums::eHerbivore){
        feedFromAutotrophs(predator, coupledSizeClassIt);
      } else if (feedingStrategy == enums::eCarnivore) {
        feedFromHeterotrophs(predator, coupledSizeClassIt);
      }
    }
  });
}

double EncounterAlgorithm::calcFeedingProbability(std::vector<SizeClass>& sizeClasses, SizeClass& thisSizeClass,
                                                  std::vector<SizeClass>::iterator& coupledSizeClassIt,
                                                  enums::eFeedingStrategy& feedingStrategy) {
  double feedingProbability = 0;
  if (thisSizeClass.getHeterotrophs().getLivingCount() != 0) {
    std::vector<double> effectiveSizeClassVolumes(numberOfSizeClasses_, 0);
    PreyVolumes preyVolumes = calcEffectiveSizeClassVolumes(sizeClasses, thisSizeClass, effectiveSizeClassVolumes);
    coupledSizeClassIt = setCoupledSizeClass(effectiveSizeClassVolumes, sizeClasses, preyVolumes, feedingStrategy);
    // PJU FIX - Introduce option switch for functional response types?
    feedingProbability = functions_.functionalResponseNonLinear(thisSizeClass.getIndex(), preyVolumes.totalPrey);
  }
  return feedingProbability;
}

PreyVolumes EncounterAlgorithm::calcEffectiveSizeClassVolumes(std::vector<SizeClass>& sizeClasses,
							      SizeClass& thisSizeClass,
							      std::vector<double>& effectiveSizeClassVolumes) {
  PreyVolumes preyVolumes;
  auto sizeClassVolumesIt = interSizeClassVolumes_[thisSizeClass.getIndex()].begin();
  auto sizeClassPreferencesIt = interSizeClassPreferences_[thisSizeClass.getIndex()].begin();
  std::vector<double>::iterator effectiveSizeClassVolumesIt = effectiveSizeClassVolumes.begin();

  std::for_each(std::begin(sizeClasses), std::end(sizeClasses), [&](SizeClass& otherSizeClass) {
    std::size_t populationSize = thisSizeClass.getHeterotrophs().getLivingCount();
    if (&thisSizeClass == &otherSizeClass) {
      populationSize--;  // Reduce population size for a single individual for this size class.
    }
    double autotrophVolume = *sizeClassPreferencesIt * otherSizeClass.getAutotrophs().getVolume();
    double totalVolume = (*sizeClassVolumesIt * populationSize) + autotrophVolume;

    *effectiveSizeClassVolumesIt = totalVolume;

    preyVolumes.autotroph += autotrophVolume;
    preyVolumes.totalPrey += totalVolume;

    std::advance(effectiveSizeClassVolumesIt, 1);
    std::advance(sizeClassPreferencesIt, 1);
    std::advance(sizeClassVolumesIt, 1);
  });
  return preyVolumes;
}

std::vector<SizeClass>::iterator EncounterAlgorithm::setCoupledSizeClass(
                                                                const std::vector<double>& effectiveSizeClassVolumes,
                                                                std::vector<SizeClass>& sizeClasses,
                                                                PreyVolumes& preyVolumes,
                                                                enums::eFeedingStrategy& feedingStrategy) {
  // Default to largest populated size class to prevent un-earned predation from taking place
  std::vector<SizeClass>::iterator coupledSizeClassIt = sizeClasses.begin();
  // Add noise to the threshold to encourage a mixed strategy (mixotroph)
  double randEffectivePreyValue = random_.getUniform() * preyVolumes.totalPrey;
  double effectivePreySum = 0;
  std::int32_t sizeClassOffset = 0;
  // Return from find_if is not used - used here to mimic break statement in classic for
  std::find_if (begin(effectiveSizeClassVolumes), end(effectiveSizeClassVolumes), [&](double effectiveSizeClassVolume) {
    effectivePreySum += effectiveSizeClassVolume;
    if (effectivePreySum >= randEffectivePreyValue) {
      std::advance(coupledSizeClassIt, sizeClassOffset);
      // Set feeding strategy here
      feedingStrategy = enums::eCarnivore;
      if (coupledSizeClassIt->getIndex() == consts::kAutotrophSizeIndex && preyVolumes.autotroph > 0) {
        double probHerbivory = preyVolumes.autotroph / preyVolumes.totalPrey;
        if (random_.getUniform() <= probHerbivory) {
          feedingStrategy = enums::eHerbivore;
        }
      }
      return true;
    } else {
      ++sizeClassOffset;
      return false;
    }
  });
  return coupledSizeClassIt;
}

void EncounterAlgorithm::feedFromHeterotrophs(Heterotroph& predator,
                                              std::vector<SizeClass>::iterator coupledSizeClassIt) {
  if (coupledSizeClassIt->getHeterotrophs().getLivingCount() != 0) {
    std::uint32_t randIdxCopy = 0;  // Copy random index for fast removal.
    Heterotroph& prey = coupledSizeClassIt->getHeterotrophs().getRandomHeterotroph(randIdxCopy);
    while(&predator == &prey) {  // Predators cannot eat themselves
      prey = coupledSizeClassIt->getHeterotrophs().getRandomHeterotroph(randIdxCopy);
    }
    double preyVolume = prey.getVolumeActual();
    double waste = predator.consumePreyVolume(preyVolume);
    nutrient_.addToVolume(waste);
    coupledSizeClassIt->getHeterotrophs().removeHeterotroph(randIdxCopy);
  }
}

void EncounterAlgorithm::feedFromAutotrophs(Heterotroph& grazer, std::vector<SizeClass>::iterator coupledSizeClassIt) {
  Autotrophs& autotrophs = coupledSizeClassIt->getAutotrophs();
  if (autotrophs.getVolume() > autotrophCellSize_) {
    autotrophs.subtractFromVolume(autotrophCellSize_);
    double waste = grazer.consumePreyVolume(autotrophCellSize_);
    nutrient_.addToVolume(waste);
  }
}
