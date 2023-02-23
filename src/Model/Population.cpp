#include "Population.h"

#include <algorithm>
#include <iostream>

#include "InitialState.h"
#include "Parameters.h"

Population::Population(Nutrient& nutrient, Autotrophs& autotrophs, unsigned numberOfSizeClasses) :
    nutrient_(nutrient),
    autotrophs_(autotrophs),
    random_(Parameters::Get()->getRandomSeed()) {
  for(unsigned index = 0; index < numberOfSizeClasses; ++index) {
    sizeClasses_.push_back(SizeClass(random_,
                                     Parameters::Get()->getSizeClassMidPoint(index),
                                     Parameters::Get()->getMaximumSizeClassPopulation(index)));
  }
  createInitialPopulation();
}

void Population::createInitialPopulation() {
 if (InitialState::Get()->isInitialised() == false) {
    unsigned initialPopulationSize = 0;
    double secondaryProducerVolume = Parameters::Get()->getSmallestIndividualVolume() * Parameters::Get()->getPreferredPreyVolumeRatio();

    unsigned firstPopulatedIndex = heterotrophProcessor_.findSizeClassIndexFromVolume(secondaryProducerVolume);
    double individualVolume = Parameters::Get()->getSizeClassMidPoint(firstPopulatedIndex);
    double traitValue = heterotrophProcessor_.volumeToTraitValue(individualVolume);

    double initialHeterotrophVolume = Parameters::Get()->getInitialHeterotrophVolume();

    while (individualVolume <= initialHeterotrophVolume) {
      initialHeterotrophVolume -= individualVolume;
      std::vector<double> traitValues{traitValue};
      std::vector<bool> areTraitsMutant{false};
      Traits traits(traitValues, areTraitsMutant);
      //individuals_[firstPopulatedIndex].push_back(new Heterotroph(heritableTraits, individualVolume));
      ++initialPopulationSize;
    }

    if (initialHeterotrophVolume > 0) nutrient_.addToVolume(initialHeterotrophVolume);

    std::cout << "A single heterotrophic size class initialised with " << initialPopulationSize + 1 << " individuals."
              << std::endl;
  } else {
    //individuals_ = InitialState::Get()->getHeterotrophs();
    std::cout << "Heterotrophic size classes initialised with " << InitialState::Get()->getInitialPopulationSize()
              << " individuals." << std::endl;
  }
}

void Population::update() {
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_),
  [] (SizeClass& sizeClass)
  {
    sizeClass.update();
  });
}

bool Population::recordData() {
  return true;
}
