#include "Heterotrophs.h"

#include <algorithm>
#include <iostream>

#include "Autotrophs.h"
#include "HeritableTraits.h"
#include "Heterotroph.h"
#include "InitialState.h"
#include "Nutrient.h"
#include "Parameters.h"

Heterotrophs::Heterotrophs(Nutrient& nutrient, Autotrophs& autotrophs)
    : nutrient_(nutrient),
      autotrophs_(autotrophs),
      heterotrophProcessor_(),
      heterotrophData_(),
      timer_(),
      interSizeClassPreferenceMatrix_(Parameters::Get()->getInterSizeClassPreferenceMatrix()),
      interSizeClassVolumeMatrix_(Parameters::Get()->getInterSizeClassVolumeMatrix()),
      sizeClassMidPoints_(Parameters::Get()->getSizeClassMidPoints()),
      maximumSizeClassPopulations_(Parameters::Get()->getMaximumSizeClassPopulations()),
      smallestIndividualVolume_(Parameters::Get()->getSmallestIndividualVolume()),
      initialHeterotrophicVolume_(Parameters::Get()->getInitialHeterotrophVolume()),
      sizeClassSubsetFraction_(Parameters::Get()->getSizeClassSubsetFraction()),
      numberOfSizeClasses_(Parameters::Get()->getNumberOfSizeClasses()),
      preferredPreyVolumeRatio_(Parameters::Get()->getPreferredPreyVolumeRatio()),
      autotrophSizeClassIndex_(Parameters::Get()->getAutotrophSizeClassIndex()) {
  createInitialPopulation();
}

Heterotrophs::~Heterotrophs() {
  for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
    for (std::size_t individualIndex = 0; individualIndex < getSizeClassPopulation(sizeClassIndex); ++individualIndex)
      delete individuals_[sizeClassIndex][individualIndex];

    individuals_[sizeClassIndex].clear();
  }
  individuals_.clear();
}

void Heterotrophs::createInitialPopulation() {
  individuals_.resize(numberOfSizeClasses_);
  individualsDead_.resize(numberOfSizeClasses_);
  fedCount_.resize(numberOfSizeClasses_);

  for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
    individuals_[sizeClassIndex].reserve(maximumSizeClassPopulations_[sizeClassIndex]);
  }

  if (InitialState::Get()->isInitialised() == false) {
    unsigned initialPopulationSize = 0;
    double secondaryProducerVolume = smallestIndividualVolume_ * preferredPreyVolumeRatio_;

    unsigned firstPopulatedIndex = heterotrophProcessor_.findSizeClassIndexFromVolume(secondaryProducerVolume);
    double individualVolume = sizeClassMidPoints_[firstPopulatedIndex];
    double traitValue = heterotrophProcessor_.volumeToTraitValue(individualVolume);

    double initialHeterotrophVolume = initialHeterotrophicVolume_;

    while (individualVolume <= initialHeterotrophVolume) {
      initialHeterotrophVolume -= individualVolume;
      std::vector<double> heritableTraitValues{traitValue};
      std::vector<bool> areTraitsMutant{false};
      HeritableTraits heritableTraits(heritableTraitValues, areTraitsMutant);
      individuals_[firstPopulatedIndex].push_back(
          new Heterotroph(heritableTraits, individualVolume, firstPopulatedIndex));
      ++initialPopulationSize;
    }

    if (initialHeterotrophVolume > 0) nutrient_.addToVolume(initialHeterotrophVolume);

    std::cout << "A single heterotrophic size class initialised with " << initialPopulationSize + 1 << " individuals."
              << std::endl;
  } else {
    individuals_ = InitialState::Get()->getHeterotrophs();
    std::cout << "Heterotrophic size classes initialised with " << InitialState::Get()->getInitialPopulationSize()
              << " individuals." << std::endl;
  }
}

void Heterotrophs::update() {
  // Feeding
  timer_.go();
  feeding();
  heterotrophData_.addToTimeFeeding(timer_.stop());
  // Metabolising
  timer_.go();
  metabolisation();
  heterotrophData_.addToTimeMetabolising(timer_.stop());
  // Starving
  timer_.go();
  starvation();
  heterotrophData_.addToTimeStarving(timer_.stop());
  // Reproducing
  timer_.go();
  reproduction();
  heterotrophData_.addToTimeReproducing(timer_.stop());
}

bool Heterotrophs::recordData() {
  heterotrophData_.initialiseDataStructures();
  for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
    std::size_t sizeClassPopulation = getSizeClassPopulation(sizeClassIndex);
    for (std::size_t individualIndex = 0; individualIndex < sizeClassPopulation; ++individualIndex) {
      heterotrophData_.addIndividualData(individuals_[sizeClassIndex][individualIndex]);
    }
    double sizeClassMultiplier = 1 / (double)sizeClassPopulation;
    heterotrophData_.addSizeClassData(sizeClassIndex, sizeClassPopulation, sizeClassMultiplier);
  }
  heterotrophData_.normaliseData();
  heterotrophData_.recordOutputData();

  return heterotrophData_.areHeterotrophsAlive();
}

std::size_t Heterotrophs::getSizeClassPopulation(const unsigned sizeClassIndex) const {
  return individuals_[sizeClassIndex].size();
}

Heterotroph* Heterotrophs::getIndividual(const unsigned sizeClassIndex, const unsigned individualIndex) {
  return individuals_[sizeClassIndex][individualIndex];
}

void Heterotrophs::calculateFeedingProbabilities() {
  for (unsigned predatorIndex = 0; predatorIndex < numberOfSizeClasses_; ++predatorIndex) {
    fedCount_[predatorIndex] = 0;
    if (getSizeClassPopulation(predatorIndex) > 0) {
      double effectivePreyVolume = 0;
      double highestEffectiveSizeClassVolume = 0;
      unsigned coupledSizeClassIndex = 0;

      for (unsigned preyIndex = 0; preyIndex < numberOfSizeClasses_; ++preyIndex) {
        double effectiveSizeClassVolume = 0;
        // Add the result of the autotroph volume - no frequency coefficient.
        if (preyIndex == autotrophSizeClassIndex_)
          effectiveSizeClassVolume =
              interSizeClassPreferenceMatrix_[predatorIndex][preyIndex] * autotrophs_.getVolume();
        // Add the result of the heterotrophs.
        std::size_t preySizeClassPopulation = getSizeClassPopulation(preyIndex);
        if (preySizeClassPopulation > 0) {
          if (preyIndex != predatorIndex)
            effectiveSizeClassVolume += interSizeClassVolumeMatrix_[predatorIndex][preyIndex] * preySizeClassPopulation;
          else
            effectiveSizeClassVolume +=
                interSizeClassVolumeMatrix_[predatorIndex][preyIndex] * (preySizeClassPopulation - 1);
        }
        if (effectiveSizeClassVolume > highestEffectiveSizeClassVolume) {
          highestEffectiveSizeClassVolume = effectiveSizeClassVolume;
          coupledSizeClassIndex = preyIndex;
        }
        heterotrophData_.setEffectiveSizeClassVolume(predatorIndex, preyIndex, effectiveSizeClassVolume);
        effectivePreyVolume += effectiveSizeClassVolume;
      }
      heterotrophData_.setEffectivePreyVolume(predatorIndex, effectivePreyVolume);
      heterotrophData_.setFeedingProbability(
          predatorIndex, heterotrophProcessor_.calculateFeedingProbability(predatorIndex, effectivePreyVolume));
      heterotrophData_.setCoupledSizeClassIndex(predatorIndex, coupledSizeClassIndex);
    }
  }
}

void Heterotrophs::feeding() {
  calculateFeedingProbabilities();

  for (unsigned predatorIndex = 0; predatorIndex < numberOfSizeClasses_; ++predatorIndex) {
    std::size_t sizeClassPopulation = getSizeClassPopulation(predatorIndex);

    if (sizeClassPopulation > 0) {
      unsigned sizeClassPopulationSubset =
          heterotrophProcessor_.roundWithProbability(random_, sizeClassPopulation * sizeClassSubsetFraction_);

      for (unsigned potentialEncounterIndex = 0; potentialEncounterIndex < sizeClassPopulationSubset;
           ++potentialEncounterIndex) {
        if (random_.getUniform() <= heterotrophData_.getFeedingProbability(predatorIndex)) {
          Heterotroph* predator = getRandomPredatorFromSizeClass(predatorIndex);
          if (predator != nullptr) {
            unsigned coupledIndex = heterotrophData_.getCoupledSizeClassIndex(predatorIndex);
            if (coupledIndex == autotrophSizeClassIndex_)
              feedFromAutotrophs(predator);
            else
              feedFromHeterotrophs(predator, coupledIndex);
          }
        }
      }
    }
  }
  deleteDead();
}

void Heterotrophs::metabolisation() {
  for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
    for (std::size_t individualIndex = 0; individualIndex < getSizeClassPopulation(sizeClassIndex); ++individualIndex) {
      Heterotroph* individual = getIndividual(sizeClassIndex, individualIndex);

      double metabolicDeduction = heterotrophProcessor_.calculateMetabolicDeduction(individual);

      if ((individual->getVolumeActual() - metabolicDeduction) > 0) {
        individual->setHasFed(false);  // Reset for the next time step
        double waste = individual->metabolise(metabolicDeduction);
        nutrient_.addToVolume(waste);

        // Individuals can move up a size class from having consumed a
        // lot. They need to move after this function has completed to
        // avoid handling them twice.
        if (heterotrophProcessor_.updateSizeClassIndex(individual) == true) stageForMoving(individual, sizeClassIndex);

      } else
        starve(individual);
    }
  }
  moveIndividuals();
  deleteDead();
}

void Heterotrophs::starvation() {
  for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
    std::size_t sizeClassPopulation = getSizeClassPopulation(sizeClassIndex);

    if (sizeClassPopulation > 0) {
      unsigned sizeClassSubsetSize =
          heterotrophProcessor_.roundWithProbability(random_, sizeClassPopulation * sizeClassSubsetFraction_);

      for (unsigned potentialStarvation = 0; potentialStarvation < sizeClassSubsetSize; ++potentialStarvation) {
        Heterotroph* individual = getRandomIndividualFromSizeClass(sizeClassIndex);

        if (individual != nullptr)
          if (random_.getUniform() <= heterotrophProcessor_.calculateStarvationProbability(individual))
            starve(individual);
      }
    }
  }
  deleteDead();
}

void Heterotrophs::reproduction() {
  for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
    for (std::size_t individualIndex = 0; individualIndex < getSizeClassPopulation(sizeClassIndex); ++individualIndex) {
      Heterotroph* individual = getIndividual(sizeClassIndex, individualIndex);
      if (individual->getVolumeActual() >= individual->getVolumeReproduction()) {
        Heterotroph* childIndividual = individual->reproduce(random_, heterotrophProcessor_);

        // Parent data needs to be collected before size class is updated.
        heterotrophData_.incrementParentFrequencies(individual->getSizeClassIndex());
        if (heterotrophProcessor_.updateSizeClassIndex(individual) == true) stageForMoving(individual, sizeClassIndex);

        if (childIndividual->getHeritableTraits().isTraitMutant(constants::eVolume) == true) {
          heterotrophProcessor_.updateSizeClassIndex(
              childIndividual);  // Mutants may inherit more than a fixed fraction of the parent's volume
          heterotrophData_.incrementMutantFrequency(childIndividual->getSizeClassIndex(), constants::eVolume);
        }

        heterotrophData_.incrementChildFrequencies(childIndividual->getSizeClassIndex());
        children_.push_back(childIndividual);
      }
    }
  }
  moveIndividuals();
  addChildren();
}

void Heterotrophs::feedFromAutotrophs(Heterotroph* grazer) {
  if (autotrophs_.getVolume() > smallestIndividualVolume_) {
    autotrophs_.subtractFromVolume(smallestIndividualVolume_);
    heterotrophData_.incrementVegetarianFrequencies(grazer);

    double waste = grazer->consumePreyVolume(smallestIndividualVolume_);
    ++fedCount_[grazer->getSizeClassIndex()];

    heterotrophProcessor_.updateHerbivoreTrophicIndex(grazer);
    nutrient_.addToVolume(waste);
  }
}

void Heterotrophs::feedFromHeterotrophs(Heterotroph* predator, const unsigned coupledIndex) {
  Heterotroph* prey = getRandomPreyFromSizeClass(coupledIndex, predator);
  if (prey != nullptr) {
    double preyVolume = prey->getVolumeActual();
    heterotrophData_.incrementCarnivoreFrequencies(predator, prey);

    double waste = predator->consumePreyVolume(preyVolume);
    ++fedCount_[predator->getSizeClassIndex()];

    heterotrophProcessor_.updateCarnivoreTrophicIndex(predator, prey);
    nutrient_.addToVolume(waste);
    kill(prey);
  }
}

// Used for starvation. Hence check against number of dead
Heterotroph* Heterotrophs::getRandomIndividualFromSizeClass(const unsigned sizeClassIndex) {
  std::size_t numberLiving = getSizeClassPopulation(sizeClassIndex);
  unsigned numberActive = numberLiving - individualsDead_[sizeClassIndex].size();

  Heterotroph* randomIndividual = nullptr;
  if (numberActive > 0) {
    unsigned count = 0;
    do {
      ++count;
      unsigned randomIndividualIndex = random_.getUniformInt(numberLiving - 1);
      randomIndividual = getIndividual(sizeClassIndex, randomIndividualIndex);
      if (count > numberActive) break;
    } while (randomIndividual->isDead() == true);
  }
  return randomIndividual;
}

// Potential predators should feed only once in a discrete time step. Not likely, but they may have already been marked
// as dead.
Heterotroph* Heterotrophs::getRandomPredatorFromSizeClass(const unsigned sizeClassIndex) {
  std::size_t numberLiving = getSizeClassPopulation(sizeClassIndex);
  unsigned numberActive = numberLiving - individualsDead_[sizeClassIndex].size() - fedCount_[sizeClassIndex];

  Heterotroph* randomIndividual = nullptr;
  if (numberActive > 0) {
    unsigned count = 0;
    do {
      ++count;
      unsigned randomIndividualIndex = random_.getUniformInt(numberLiving - 1);
      randomIndividual = getIndividual(sizeClassIndex, randomIndividualIndex);
      if (count > numberActive) break;
    } while (randomIndividual->isDead() == true || randomIndividual->hasFed() == true);
  }
  return randomIndividual;
}

// Potential prey may already be marked as dead. They cannot be eaten by themselves.
Heterotroph* Heterotrophs::getRandomPreyFromSizeClass(const unsigned sizeClassIndex,
                                                      const Heterotroph* predator) {
  std::size_t numberLiving = getSizeClassPopulation(sizeClassIndex);
  unsigned numberActive = numberLiving - individualsDead_[sizeClassIndex].size();

  // Only applicable when predator feeds from its own size class. Check
  // ensures numberActive does not go out of bounds
  if (predator->getSizeClassIndex() == sizeClassIndex && numberActive > 0) --numberActive;

  Heterotroph* randomIndividual = nullptr;
  if (numberActive > 0) {
    unsigned count = 0;
    do {
      ++count;
      unsigned randomIndividualIndex = random_.getUniformInt(numberLiving - 1);
      randomIndividual = getIndividual(sizeClassIndex, randomIndividualIndex);
      if (count > numberActive) break;
    } while (randomIndividual->isDead() == true || randomIndividual == predator);
  }
  return randomIndividual;
}

void Heterotrophs::stageForMoving(Heterotroph* individual, const unsigned currentSizeClassIndex) {
  individualsToMove_.push_back(std::make_pair(individual, currentSizeClassIndex));
}

void Heterotrophs::moveSizeClass(Heterotroph* individual, const unsigned currentSizeClassIndex) {
  if (removeFromSizeClass(individual, currentSizeClassIndex) == true)
    individuals_[individual->getSizeClassIndex()].push_back(individual);
}

bool Heterotrophs::removeFromSizeClass(Heterotroph* individual, const unsigned sizeClassIndex) {
  std::vector<Heterotroph*>::iterator toErase;
  toErase = std::find(individuals_[sizeClassIndex].begin(), individuals_[sizeClassIndex].end(), individual);

  // And then erase if found
  if (toErase != individuals_[sizeClassIndex].end()) {
    // Delete when necessary in calling function
    individuals_[sizeClassIndex].erase(toErase);
    return true;
  }
  return false;
}

void Heterotrophs::deleteIndividual(Heterotroph* individual) {
  removeFromSizeClass(individual, individual->getSizeClassIndex());
  delete individual;
}

void Heterotrophs::starve(Heterotroph* individual) {
  nutrient_.addToVolume(individual->getVolumeActual());
  heterotrophData_.incrementStarvedFrequencies(individual->getSizeClassIndex());
  kill(individual);
}

void Heterotrophs::kill(Heterotroph* individual) {
  individual->setDead();
  individualsDead_[individual->getSizeClassIndex()].push_back(individual);
}

void Heterotrophs::addChildren() {
  for (std::size_t childIndex = 0; childIndex < children_.size(); ++childIndex) {
    Heterotroph* child = children_[childIndex];
    individuals_[child->getSizeClassIndex()].push_back(child);
  }
  children_.clear();
}

void Heterotrophs::moveIndividuals() {
  for (unsigned index = 0; index < individualsToMove_.size(); ++index) {
    moveSizeClass(individualsToMove_[index].first, individualsToMove_[index].second);
  }
  individualsToMove_.clear();
}

void Heterotrophs::deleteDead() {
  for (unsigned sizeClassIndex = 0; sizeClassIndex < numberOfSizeClasses_; ++sizeClassIndex) {
    for (unsigned individualIndex = 0; individualIndex < individualsDead_[sizeClassIndex].size(); ++individualIndex)
      deleteIndividual(individualsDead_[sizeClassIndex][individualIndex]);

    individualsDead_[sizeClassIndex].clear();
  }
}
