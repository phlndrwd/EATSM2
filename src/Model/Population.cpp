#include "Population.h"

#include <algorithm>
#include <iostream>
#include <climits>

#include "InitialState.h"
#include "Parameters.h"

Population::Population(Nutrient& nutrient, Autotrophs& autotrophs, unsigned numberOfSizeClasses) :
    nutrient_(nutrient),
    autotrophs_(autotrophs),
    random_(Parameters::Get()->getRandomSeed()) {
  for(unsigned index = 0; index < numberOfSizeClasses; ++index) {
    sizeClasses_.push_back(SizeClass(heterotrophData_,
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
    if(predatorSizeClass.getPopulationSize() != 0) {
      double effectivePreyVolume = 0;
      double highestEffectiveSizeClassVolume = 0;
      unsigned coupledSizeClassIndex = 0;
      std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_),
      [&](SizeClass& preySizeClass)
      {

      });
    }
  });

//  for (unsigned predatorIndex = 0; predatorIndex < numberOfSizeClasses_; ++predatorIndex) {
//    fedCount_[predatorIndex] = 0;
//    if (getSizeClassPopulation(predatorIndex) > 0) {
//      double effectivePreyVolume = 0;
//      double highestEffectiveSizeClassVolume = 0;
//      unsigned coupledSizeClassIndex = 0;

//      for (unsigned preyIndex = 0; preyIndex < numberOfSizeClasses_; ++preyIndex) {
//        double effectiveSizeClassVolume = 0;
//        // Add the result of the autotroph volume - no frequency coefficient.
//        if (preyIndex == autotrophSizeClassIndex_)
//          effectiveSizeClassVolume =
//              interSizeClassPreferenceMatrix_[predatorIndex][preyIndex] * autotrophs_.getVolume();
//        // Add the result of the heterotrophs.
//        std::size_t preySizeClassPopulation = getSizeClassPopulation(preyIndex);
//        if (preySizeClassPopulation > 0) {
//          if (preyIndex != predatorIndex)
//            effectiveSizeClassVolume += interSizeClassVolumeMatrix_[predatorIndex][preyIndex] * preySizeClassPopulation;
//          else
//            effectiveSizeClassVolume +=
//                interSizeClassVolumeMatrix_[predatorIndex][preyIndex] * (preySizeClassPopulation - 1);
//        }
//        if (effectiveSizeClassVolume > highestEffectiveSizeClassVolume) {
//          highestEffectiveSizeClassVolume = effectiveSizeClassVolume;
//          coupledSizeClassIndex = preyIndex;
//        }
//        heterotrophData_.setEffectiveSizeClassVolume(predatorIndex, preyIndex, effectiveSizeClassVolume);
//        effectivePreyVolume += effectiveSizeClassVolume;
//      }
//      heterotrophData_.setEffectivePreyVolume(predatorIndex, effectivePreyVolume);
//      heterotrophData_.setFeedingProbability(
//          predatorIndex, heterotrophProcessor_.calculateFeedingProbability(predatorIndex, effectivePreyVolume));
//      heterotrophData_.setCoupledSizeClassIndex(predatorIndex, coupledSizeClassIndex);
//    }
//  }
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
