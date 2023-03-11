#include "Heterotrophs.h"

#include <algorithm>
#include <iostream>
#include <climits>

#include "Parameters.h"

Heterotrophs::Heterotrophs(Nutrient& nutrient, Autotrophs& autotrophs, const unsigned numberOfSizeClasses) :
    nutrient_(nutrient),
    autotrophs_(autotrophs),
    numberOfSizeClasses_(numberOfSizeClasses),
    random_(Parameters::Get()->getRandomSeed()),
    sizeClasses_(numberOfSizeClasses_, SizeClass(1, random_.getUniformInt(1, UINT_MAX))) {

  //sizeClasses_.resize(numberOfSizeClasses_);
//  unsigned index = 0;
//  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_),
//  [&](SizeClass& sizeClass) {
//    sizeClass = SizeClass(heterotrophData_, index, random_.getUniformInt(1, UINT_MAX));
//    index++;
//  });

//  for(unsigned index = 0; index < numberOfSizeClasses_; ++index) {
//    sizeClasses_.push_back(SizeClass(heterotrophData_, index, random_.getUniformInt(1, UINT_MAX)));
//  }
  createInitialPopulation();
}

void Heterotrophs::createInitialPopulation() {
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

void Heterotrophs::calculateFeedingProbabilities() {
  std::vector<size_t> populationSizes(numberOfSizeClasses_, 0);
  auto popSizesIt = populationSizes.begin();
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_),
  [&](SizeClass& sizeClass) {
    *popSizesIt = sizeClass.getPopulationSize();
    ++popSizesIt;
  });
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_),
  [&](SizeClass& sizeClass) {
     sizeClass.calculateFeedingProbability(populationSizes);
  });
}

void Heterotrophs::update() {
  calculateFeedingProbabilities();
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

bool Heterotrophs::recordData() {
  return true;
}
