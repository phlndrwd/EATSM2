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

EncounterAlgorithm::EncounterAlgorithm(Autotrophs* autotrophs, Nutrient* nutrient, Parameters* params,
                                       const std::uint32_t& randomSeed) :
    autotrophs_(autotrophs),
    nutrient_(nutrient),
    functions_(*params),
    random_(randomSeed),
    interSizeClassPreferences_(params->getInterSizeClassPreferences()),
    interSizeClassVolumes_(params->getInterSizeClassVolumes()),
    numberOfSizeClasses_(params->getNumberOfSizeClasses()),
    autotrophSizeIndex_(consts::kAutotrophSizeIndex),
    autotrophCellSize_(params->getAutotrophCellSize()) {}

void EncounterAlgorithm::update(std::vector<SizeClass>& sizeClasses) {
  std::for_each(std::begin(sizeClasses), std::end(sizeClasses), [&](SizeClass& thisSizeClass) {
    eFeedingStrategy feedingStrategy = eNotEating;
    std::uint32_t coupledSizeClassIndex = 0;
    std::float64_t feedingProbability = calcFeedingProbability(sizeClasses, thisSizeClass, coupledSizeClassIndex, feedingStrategy);
    SizeClass& coupledSizeClass = sizeClasses[coupledSizeClassIndex];
    thisSizeClass.getHeterotrophs().subset(random_, [&](const std::uint32_t index) {
      if (random_.getUniform() <= feedingProbability) {
        Heterotroph& predator = thisSizeClass.getHeterotroph(index);

        if (feedingStrategy == eHerbivore){
          feedFromAutotrophs(predator);
        } else if (feedingStrategy == eCarnivore) {
          feedFromHeterotrophs(predator, coupledSizeClass);
        }
      }
    });
    coupledSizeClass.removeDead();
  });
}

std::float64_t EncounterAlgorithm::calcFeedingProbability(std::vector<SizeClass>& sizeClasses, SizeClass& thisSizeClass,
                                                          std::uint32_t& coupledSizeClassIndex,
                                                          eFeedingStrategy& feedingStrategy) {
  std::float64_t feedingProbability = 0;
  if (thisSizeClass.getPopulationSize() != 0) {
    std::vector<std::float64_t> effectivePreyVolumes(numberOfSizeClasses_, 0);
    PreyVolumes preyVolumes = calcEffectivePreyVolumes(sizeClasses, thisSizeClass, effectivePreyVolumes);
    coupledSizeClassIndex = setCoupledSizeClassIndex(effectivePreyVolumes, preyVolumes, feedingStrategy);
    feedingProbability = functions_.functionalResponse(thisSizeClass.getIndex(), preyVolumes.total);
  }
  return feedingProbability;
}

PreyVolumes EncounterAlgorithm::calcEffectivePreyVolumes(std::vector<SizeClass>& sizeClasses,
                                                        SizeClass& thisSizeClass,
                                                        std::vector<std::float64_t>& effectivePreyVolumes) {
  PreyVolumes preyVolumes;
  for(std::uint32_t preyIndex = 0; preyIndex < numberOfSizeClasses_; ++preyIndex) {
    SizeClass& otherSizeClass = sizeClasses[preyIndex];
    std::size_t populationSize = otherSizeClass.getPopulationSize();
    if (&thisSizeClass == &otherSizeClass) {
      populationSize--;  // Reduce population size for a single individual for this size class.
    }
    std::float64_t autotrophVolume = 0;
    if (preyIndex == autotrophSizeIndex_) {
      std::float64_t sizeClassPreference = interSizeClassPreferences_[thisSizeClass.getIndex()][preyIndex];
      autotrophVolume = autotrophs_->getVolume() * sizeClassPreference;
      preyVolumes.autotroph = autotrophVolume;
    }
    std::float64_t heterotrophVolume = 0;
    if (populationSize != 0) {
      std::float64_t sizeClassVolume = interSizeClassVolumes_[thisSizeClass.getIndex()][preyIndex];
      heterotrophVolume = sizeClassVolume * populationSize;
    }
    effectivePreyVolumes[preyIndex] = heterotrophVolume + autotrophVolume;;
    preyVolumes.total += effectivePreyVolumes[preyIndex];
  }
  return preyVolumes;
}

std::uint32_t EncounterAlgorithm::setCoupledSizeClassIndex(
                                    const std::vector<std::float64_t>& effectiveSizeClassVolumes,
                                    PreyVolumes& preyVolumes,
                                    eFeedingStrategy& feedingStrategy) {
  std::uint32_t coupledSizeClassIndex = 0;
  // Add noise to encourage a mixed strategy
  std::float64_t randEffectivePreyValue = random_.getUniform() * preyVolumes.total;
  std::float64_t effectivePreySum = 0;
  for (std::size_t i = 0; i < numberOfSizeClasses_; ++i) {
    effectivePreySum += effectiveSizeClassVolumes[i];

    if (effectivePreySum >= randEffectivePreyValue) {
      coupledSizeClassIndex = i;
      feedingStrategy = eCarnivore;  // Default feeing strategy
      // Possibly switch to herbivore if this is autotroph size class
      if (i == consts::kAutotrophSizeIndex && preyVolumes.autotroph > 0) {
        std::float64_t probHerbivory = preyVolumes.autotroph / preyVolumes.total;
        if (random_.getUniform() <= probHerbivory) {
          feedingStrategy = eHerbivore;
        }
      }
      break;
    }
  }
  return coupledSizeClassIndex;
}

void EncounterAlgorithm::feedFromHeterotrophs(Heterotroph& predator,
                                              SizeClass& coupledSizeClass) {
  if (coupledSizeClass.getPopulationSize() != 0) {
    std::uint32_t preyIndex;
    Heterotroph& prey = coupledSizeClass.getRandomHeterotroph(preyIndex);
    // Predators cannot eat themselves or eat those who've already been eaten
    while(&predator == &prey || prey.isAlive() == false) {
      prey = coupledSizeClass.getRandomHeterotroph(preyIndex);
    }
    std::float64_t preyVolume = prey.getVolumeActual();
    std::float64_t waste = predator.consumePreyVolume(preyVolume);
    nutrient_->addToVolume(waste);
    prey.kill();
    coupledSizeClass.killHeterotroph(preyIndex);
  }
}

void EncounterAlgorithm::feedFromAutotrophs(Heterotroph& grazer) {
  if (autotrophs_->getVolume() >= autotrophCellSize_) {
    autotrophs_->subtractFromVolume(autotrophCellSize_);
    std::float64_t waste = grazer.consumePreyVolume(autotrophCellSize_);
    nutrient_->addToVolume(waste);
  }
}
