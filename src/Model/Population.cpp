#include "Population.h"

#include <algorithm>
#include <iostream>
#include <climits>

#include "InitialState.h"
#include "Parameters.h"

Population::Population(Nutrient& nutrient, Autotrophs& autotrophs, const unsigned numberOfSizeClasses) :
    nutrient_(nutrient),
    autotrophs_(autotrophs),
    random_(Parameters::Get()->getRandomSeed()) {
  for(unsigned index = 0; index < numberOfSizeClasses; ++index) {
    sizeClasses_.push_back(SizeClass(heterotrophData_,
                                     Parameters::Get()->getInterSizeClassPreferenceVector(index),
                                     Parameters::Get()->getInterSizeClassVolumeVector(index),
                                     Parameters::Get()->getSizeClassMidPoint(index),
                                     Parameters::Get()->getSizeClassSubsetFraction(),
                                     Parameters::Get()->getMaximumSizeClassPopulation(index),
                                     random_.getUniformInt(1, UINT_MAX)));
  }
  createInitialPopulation();
}

void Population::createInitialPopulation() {
 //if (InitialState::Get()->isInitialised() == false)
 {
    double secondaryProducerVolume = Parameters::Get()->getSmallestIndividualVolume() * Parameters::Get()->getPreferredPreyVolumeRatio();
    unsigned sizeClassIndex = heterotrophProcessor_.findSizeClassIndexFromVolume(secondaryProducerVolume);
    double individualVolume = Parameters::Get()->getSizeClassMidPoint(sizeClassIndex);
    double traitValue = heterotrophProcessor_.volumeToTraitValue(individualVolume);
    double initialHeterotrophVolume = Parameters::Get()->getInitialHeterotrophVolume();

    unsigned initialPopulationSize = 0;
    while(individualVolume <= initialHeterotrophVolume) {
      initialHeterotrophVolume -= individualVolume;
      std::vector<double> traitValues{traitValue};
      std::vector<bool> areTraitsMutant{false};
      Traits traits(traitValues, areTraitsMutant);
      Heterotroph heterotroph(std::move(traits), individualVolume);
      sizeClasses_[sizeClassIndex].addHeterotroph(std::move(heterotroph));
      ++initialPopulationSize;
    }

    if (initialHeterotrophVolume > 0) {
      nutrient_.addToVolume(initialHeterotrophVolume);
    }

    std::cout << "A single heterotrophic size class initialised with " << initialPopulationSize << " individuals."
              << std::endl;
  }
  //  else {
  //    //individuals_ = InitialState::Get()->getHeterotrophs();
  //    std::cout << "Heterotrophic size classes initialised with " << InitialState::Get()->getInitialPopulationSize()
  //              << " individuals." << std::endl;
  //  }
}

void Population::calculateFeedingProbabilities() {
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_),
  [&](SizeClass& predatorSizeClass)
  {

  });
}

void Population::update() {
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_),
  [&](SizeClass& sizeClass) {
    std::vector<Heterotroph> heterotrophsToMove = sizeClass.update(nutrient_);
    for (const auto& heterotroph : heterotrophsToMove) {
      std::vector<SizeClass>::iterator sizeClassIt = sizeClasses_.begin();
      std::advance(sizeClassIt, heterotroph.getSizeClassIndex());
      sizeClassIt->addHeterotroph(heterotroph);
    }
  });
}

bool Population::recordData() {
  return true;
}
