/******************************************************************************
* Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    *
*                                                                             *
* (C) Copyright 2024, Phil Underwood. All rights reserved.                    *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/

#include "HeterotrophPopulation.h"

#include "Constants.h"

#include <cmath>
#include <climits>
#include <memory>
#include <iostream>

namespace {

std::float64_t volumeToTraitValue(const std::float64_t& volume,
                                  const std::float64_t& smallestVolumeExponent,
                                  const std::float64_t& largestVolumeExponent) {
  return (std::log10(volume) - smallestVolumeExponent) /
         (largestVolumeExponent - smallestVolumeExponent);
}


std::unique_ptr<Heterotroph> heterotrophGenerator(std::float64_t traitValue,
                                                  std::float64_t volume,
                                                  std::float64_t assimilationEfficiency,
                                            const std::float64_t mutationProbability,
                                            const std::float64_t mutationStandardDeviation) {
  return std::make_unique<Heterotroph>(traitValue, mutationProbability, mutationStandardDeviation, volume, assimilationEfficiency);
}

}  // Anonymous namespace

HeterotrophPopulation::HeterotrophPopulation(Nutrient* nutrient, Autotrophs* autotrophs, Parameters* params, const std::uint32_t& randomSeed) :
	nutrient_(nutrient),
        autotrophs_(autotrophs),
        functions_(*params),
        random_(randomSeed),
        interSizeClassPreferences_(params->getInterSizeClassPreferences()),
        interSizeClassVolumes_(params->getInterSizeClassVolumes()),
        smallestVolumeExponent_(params->getSmallestVolumeExponent()),
        largestVolumeExponent_(params->getLargestVolumeExponent()),
        autotrophCellSize_(params->getAutotrophCellSize()),
        numberOfSizeClasses_(params->getNumberOfSizeClasses()),
        autotrophSizeIndex_(consts::kAutotrophSizeIndex) {
  populate(params);
}

void HeterotrophPopulation::update() {
  feeding();
  metabolisation();
  starvation();
  reproduction();
}

void HeterotrophPopulation::populate(Parameters* params) {
  sizeClasses_.reserve(numberOfSizeClasses_);
  const std::float64_t initialVolume = params->getInitialHeterotrophVolume();

  std::uint32_t heterotrophIndex = params->getIndividualHeterotrophIndex();
  for (std::uint32_t index = 0; index < numberOfSizeClasses_; ++index) {

    sizeClasses_.emplace_back(params, index);  // Initialise SizeClass

    if (initialVolume > 0 && index == heterotrophIndex) {  // Populate single SizeClass
      std::float64_t sizeClassMidPoint = sizeClasses_[index].getSizeClassMidPoint();
      std::float64_t realInitialPopulationSize = initialVolume / sizeClassMidPoint;
      std::uint32_t initialPopulationSize = static_cast<std::uint32_t>(std::floor(realInitialPopulationSize));
      std::float64_t unusedVolume = (realInitialPopulationSize - initialPopulationSize) * sizeClassMidPoint;

      nutrient_->addToVolume(unusedVolume); // DEBUG AND CHECK THE CALCULATION HERE //////////////////////////////////////

      std::float64_t traitValue = volumeToTraitValue(
        sizeClassMidPoint,
        params->getSmallestVolumeExponent(),
        params->getLargestVolumeExponent()
      );

      for (auto _ = initialPopulationSize; _--;) {
        sizeClasses_[index].addHeterotroph(
          heterotrophGenerator(
            traitValue,
            sizeClassMidPoint,
            params->getAssimilationEfficiency(),
            params->getMutationProbability(),
            params->getMutationStandardDeviation()
          )
        );
      }
      std::cout << "Size class with index " << index << " initialised with " << initialPopulationSize << " heterotrophs." << std::endl;
    }
  }
}

void HeterotrophPopulation::feeding() {
  for (auto& sizeClass : sizeClasses_) {
    if (sizeClass.getLivingCount() > 0 ) {
      eFeedingStrategy feedingStrategy = eNotEating;
      std::uint32_t coupledSizeClassIndex = 0;
      std::float64_t feedingProbability = calcFeedingProbability(sizeClass, coupledSizeClassIndex, feedingStrategy);
      SizeClass& coupledSizeClass = sizeClasses_[coupledSizeClassIndex];
      sizeClass.subset(random_, [&](Heterotroph* predator) {
          if (predator) {
            if (random_.getUniform() <= feedingProbability) {
              if (feedingStrategy == eHerbivore){
                feedFromAutotrophs(predator);
              } else if (feedingStrategy == eCarnivore) {
                feedFromHeterotrophs(predator, coupledSizeClass);
              }
            }
          }
        }
      );
    }
    removeDead();
  }
}

void HeterotrophPopulation::metabolisation() {
  for (auto& sizeClass : sizeClasses_ ) {
    sizeClass.forEachHeterotroph(
      [&](std::uint32_t heterotrophIndex, Heterotroph* heterotroph) {
        std::float64_t metabolicDeduction = functions_.calcMetabolicDeduction(heterotroph);
        if ((heterotroph->getVolumeActual() - metabolicDeduction) > 0) {
          std::float64_t waste = heterotroph->metabolise(metabolicDeduction);
          nutrient_->addToVolume(waste);
        } else {
          nutrient_->addToVolume(heterotroph->getVolumeActual());
          heterotroph->kill();
        }
      }
    );
  }
  removeDead();
}

void HeterotrophPopulation::starvation() {
  for (auto& sizeClass : sizeClasses_ ) {
    sizeClass.subset(random_, [&](Heterotroph* heterotroph) {
      if (heterotroph) {
          if (random_.getUniform() <= functions_.calcStarvationProbability(heterotroph)) {
            nutrient_->addToVolume(heterotroph->getVolumeActual());
            heterotroph->kill();
          }
        }
      }
    );
  }
  removeDead();
}

void HeterotrophPopulation::reproduction() {
  for (auto& sizeClass : sizeClasses_ ) {
    sizeClass.forEachHeterotroph(
      [&](std::uint32_t heterotrophIndex, Heterotroph* heterotroph) {
        if (heterotroph->getVolumeActual() >= heterotroph->getVolumeReproduction()) {
          sizeClass.addChild(
            heterotroph->getChild(random_, smallestVolumeExponent_, largestVolumeExponent_)
          );
        }
        std::uint32_t sizeClassIndex = sizeClass.getIndex();
        if (heterotroph->getVolumeActual() < sizeClass.getSizeClassLower() && sizeClassIndex > 0) {  // Zero is smallest size class
          movingHeterotrophs_.push_back(MovingHeterotroph(sizeClass.ownHeterotroph(heterotrophIndex),
                                       sizeClassIndex, eShrinking));
        } else if (heterotroph->getVolumeActual() >= sizeClass.getSizeClassUpper() && sizeClassIndex < numberOfSizeClasses_ - 1) {
          movingHeterotrophs_.push_back(MovingHeterotroph(sizeClass.ownHeterotroph(heterotrophIndex),
                                       sizeClassIndex, eGrowing));
        }
      }
    );
  }
  for (auto& sizeClass : sizeClasses_ ) {
    sizeClass.removeDead();  // Remove gaps left by moving individuals
    sizeClass.forEachChild([&](std::unique_ptr<Heterotroph> child) {
      sizeClass.addHeterotroph(std::move(child));
    });
    sizeClass.clearChildren();
  }
}

void HeterotrophPopulation::removeDead() {
  for (auto& sizeClass : sizeClasses_ ) {
    sizeClass.removeDead();
  }
}

std::float64_t HeterotrophPopulation::calcFeedingProbability(SizeClass& thisSizeClass,
                                                             std::uint32_t& coupledSizeClassIndex,
                                                             eFeedingStrategy& feedingStrategy) {
  std::float64_t feedingProbability = 0;
  if (thisSizeClass.getLivingCount() != 0) {
    std::vector<std::float64_t> effectivePreyVolumes(numberOfSizeClasses_, 0);
    PreyVolumes preyVolumes = calcEffectivePreyVolumes(thisSizeClass, effectivePreyVolumes);
    coupledSizeClassIndex = setCoupledSizeClassIndex(effectivePreyVolumes, preyVolumes, feedingStrategy);
    feedingProbability = functions_.functionalResponse(thisSizeClass.getIndex(), preyVolumes.total);
  }
  return feedingProbability;
}

PreyVolumes HeterotrophPopulation::calcEffectivePreyVolumes(SizeClass& thisSizeClass,
                                                            std::vector<std::float64_t>& effectivePreyVolumes) {
  PreyVolumes preyVolumes;
  for(std::uint32_t preyIndex = 0; preyIndex < numberOfSizeClasses_; ++preyIndex) {
    SizeClass& otherSizeClass = sizeClasses_[preyIndex];
    std::size_t populationSize = otherSizeClass.getLivingCount();
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

std::uint32_t HeterotrophPopulation::setCoupledSizeClassIndex(
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

void HeterotrophPopulation::feedFromHeterotrophs(Heterotroph* predator,
                                                 SizeClass& coupledSizeClass) {
  if (coupledSizeClass.getLivingCount() != 0) {
    Heterotroph* prey = nullptr;
    do {
      prey = coupledSizeClass.getRandomHeterotroph(random_);
    } while(&predator == &prey || prey->isAlive() == false); // Predators cannot eat themselves or eat those who've already been eaten
    std::float64_t preyVolume = prey->getVolumeActual();
    std::float64_t waste = predator->consumePreyVolume(preyVolume);
    nutrient_->addToVolume(waste);
    prey->kill();
  }
}

void HeterotrophPopulation::feedFromAutotrophs(Heterotroph* grazer) {
  if (autotrophs_->getVolume() >= autotrophCellSize_) {
    autotrophs_->subtractFromVolume(autotrophCellSize_);
    std::float64_t waste = grazer->consumePreyVolume(autotrophCellSize_);
    nutrient_->addToVolume(waste);
  }
}

//std::float64_t HeterotrophPopulation::getVolume() const {
//  std::float64_t volume = 0;
//  for (auto& sizeClass : sizeClasses_ ) {
//      sizeClass.forEachHeterotroph([&](Heterotroph* heterotroph ) {
//      volume += heterotroph->getVolumeActual();
//    });
//  }
//  return volume;
//}

//void HeterotrophPopulation::moveHeterotrophs() {
//  for (auto& moving : movingHeterotrophs_) {
//    const auto volume = moving.heterotroph->getVolumeActual();
//    if (moving.growthTrajectory == eGrowing) {
//      for (std::uint32_t index = moving.prevSizeClassIndex + 1; index < sizeClasses_.size(); ++index) {
//        if (volume < sizeClasses_[index].getSizeClassLower()) {
//          sizeClasses_[index].addHeterotroph(std::move(moving.heterotroph));
//          break;
//        }
//      }
//    } else if (moving.growthTrajectory == eShrinking) {
//      for (std::int32_t index = static_cast<std::int32_t>(moving.prevSizeClassIndex) - 1; index >= 0;  --index) {
//        if (volume >= sizeClasses_[index].getSizeClassUpper()) {
//          sizeClasses_[index].addHeterotroph(std::move(moving.heterotroph));
//          break;
//        }
//      }
//    }
//  }
//  movingHeterotrophs_.clear();
//}
