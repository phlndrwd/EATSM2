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

EncounterAlgorithm::EncounterAlgorithm(Nutrient& nutrient, Parameters& params,
                                       const std::uint32_t& randomSeed):
    nutrient_(nutrient),
    functions_(params),
    random_(randomSeed),
    interSizeClassPreferences_(params.getInterSizeClassPreferences()),
    interSizeClassVolumes_(params.getInterSizeClassVolumes()),
    numberOfSizeClasses_(params.getNumberOfSizeClasses()),
    autotrophCellSize_(params.getAutotrophCellSize()) {}

void EncounterAlgorithm::update(std::vector<SizeClass>& sizeClasses, SizeClass& thisSizeClass) {
  eFeedingStrategy feedingStrategy = eNotEating;
  std::vector<SizeClass>::iterator coupledSizeClassIt = sizeClasses.begin();
  std::float64_t feedingProbability = calcFeedingProbability(sizeClasses, thisSizeClass, coupledSizeClassIt, feedingStrategy);
  thisSizeClass.getHeterotrophs().subset(random_, [&](const std::uint32_t randomIndex) {
    if (random_.getUniform() <= feedingProbability) {
      Heterotroph& predator = thisSizeClass.getHeterotroph(randomIndex);
      if (feedingStrategy == eHerbivore){
        feedFromAutotrophs(predator, coupledSizeClassIt);
      } else if (feedingStrategy == eCarnivore) {
        feedFromHeterotrophs(predator, coupledSizeClassIt);
      }
    }
  });
}

std::float64_t EncounterAlgorithm::calcFeedingProbability(std::vector<SizeClass>& sizeClasses, SizeClass& thisSizeClass,
                                                  std::vector<SizeClass>::iterator& coupledSizeClassIt,
                                                  eFeedingStrategy& feedingStrategy) {
  std::float64_t feedingProbability = 0;
  if (thisSizeClass.getHeterotrophs().getLivingCount() != 0) {
    std::vector<std::float64_t> effectiveSizeClassVolumes(numberOfSizeClasses_, 0);
    PreyVolumes preyVolumes = calcEffectiveSizeClassVolumes(sizeClasses, thisSizeClass, effectiveSizeClassVolumes);
    coupledSizeClassIt = setCoupledSizeClass(effectiveSizeClassVolumes, sizeClasses, preyVolumes, feedingStrategy);
    // PJU FIX - Introduce option switch for functional response types?
    feedingProbability = functions_.functionalResponse(thisSizeClass.getIndex(), preyVolumes.totalPrey);
  }
  return feedingProbability;
}

PreyVolumes EncounterAlgorithm::calcEffectiveSizeClassVolumes(std::vector<SizeClass>& sizeClasses,
							      SizeClass& thisSizeClass,
							      std::vector<std::float64_t>& effectiveSizeClassVolumes) {
  PreyVolumes preyVolumes;
  auto sizeClassVolumesIt = interSizeClassVolumes_[thisSizeClass.getIndex()].begin();
  auto sizeClassPreferencesIt = interSizeClassPreferences_[thisSizeClass.getIndex()].begin();
  std::vector<std::float64_t>::iterator effectiveSizeClassVolumesIt = effectiveSizeClassVolumes.begin();

  std::for_each(std::begin(sizeClasses), std::end(sizeClasses), [&](SizeClass& otherSizeClass) {
    std::size_t populationSize = thisSizeClass.getHeterotrophs().getLivingCount();
    if (&thisSizeClass == &otherSizeClass) {
      populationSize--;  // Reduce population size for a single individual for this size class.
    }
    std::float64_t autotrophVolume = 0; ////////////////////////////////////// *sizeClassPreferencesIt * otherSizeClass.getAutotrophs().getVolume();
    std::float64_t totalVolume = (*sizeClassVolumesIt * populationSize) + autotrophVolume;

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
                                                                const std::vector<std::float64_t>& effectiveSizeClassVolumes,
                                                                std::vector<SizeClass>& sizeClasses,
                                                                PreyVolumes& preyVolumes,
                                                                eFeedingStrategy& feedingStrategy) {
  // Default to largest populated size class to prevent un-earned predation from taking place
  std::vector<SizeClass>::iterator coupledSizeClassIt = sizeClasses.begin();
  // Add noise to the threshold to encourage a mixed strategy (mixotroph)
  std::float64_t randEffectivePreyValue = random_.getUniform() * preyVolumes.totalPrey;
  std::float64_t effectivePreySum = 0;
  std::int32_t sizeClassOffset = 0;
  // Return from find_if is not used - used here to mimic break statement in classic for
  std::find_if (begin(effectiveSizeClassVolumes), end(effectiveSizeClassVolumes), [&](std::float64_t effectiveSizeClassVolume) {
    effectivePreySum += effectiveSizeClassVolume;
    if (effectivePreySum >= randEffectivePreyValue) {
      std::advance(coupledSizeClassIt, sizeClassOffset);
      // Set feeding strategy here
      feedingStrategy = eCarnivore;
      if (coupledSizeClassIt->getIndex() == consts::kAutotrophSizeIndex && preyVolumes.autotroph > 0) {
        std::float64_t probHerbivory = preyVolumes.autotroph / preyVolumes.totalPrey;
        if (random_.getUniform() <= probHerbivory) {
          feedingStrategy = eHerbivore;
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
    std::uint32_t randIndex = coupledSizeClassIt->getRandomHeterotrophIndex();
    Heterotroph& prey = coupledSizeClassIt->getHeterotroph(randIndex);
    while(&predator == &prey) {  // Predators cannot eat themselves
      prey = coupledSizeClassIt->getHeterotroph(randIndex);
    }
    std::float64_t preyVolume = prey.getVolumeActual();
    std::float64_t waste = predator.consumePreyVolume(preyVolume);
    nutrient_.addToVolume(waste);
    coupledSizeClassIt->getHeterotrophs().removeHeterotroph(randIndex);
  }
}

void EncounterAlgorithm::feedFromAutotrophs(Heterotroph& grazer, std::vector<SizeClass>::iterator coupledSizeClassIt) {
  //Autotrophs& autotrophs = coupledSizeClassIt->getAutotrophs();
  //if (autotrophs.getVolume() > autotrophCellSize_) {
  //  autotrophs.subtractFromVolume(autotrophCellSize_);
  //  std::float64_t waste = grazer->consumePreyVolume(autotrophCellSize_);
  //  nutrient_.addToVolume(waste);
  //}
}
