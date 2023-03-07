#include "SizeClass.h"

#include <algorithm>
#include <climits>
#include <iostream>
#include <stdexcept>

#include <Parameters.h>

SizeClass::SizeClass(HeterotrophData& heterotrophData,
                     const std::vector<double> sizeClassPreferences,
                     const std::vector<double> sizeClassVolumes,
                     const double sizeClassMidPoint,
                     const double sizeClassSubsetFraction,
                     const unsigned maxPopulation,
                     const unsigned randomSeed) :
    heterotrophData_(heterotrophData),
    sizeClassPreferences_(sizeClassPreferences),
    sizeClassVolumes_(sizeClassVolumes),
    sizeClassMidPoint_(sizeClassMidPoint),
    sizeClassSubsetFraction_(sizeClassSubsetFraction),
    maxPopulation_(maxPopulation),
    random_(randomSeed),
    numberOfSizeClasses_(100),
    autotrophSizeClassIndex_(Parameters::Get()->getAutotrophSizeClassIndex()) {
  heterotrophs_.reserve(maxPopulation_);
  alive_.reserve(maxPopulation_);
}

std::vector<Heterotroph> SizeClass::update(Nutrient& nutrient) {
  std::vector<Heterotroph> heterotrophsToMove;

  metabolisation(nutrient);
  starvation(nutrient);

  return heterotrophsToMove;
}

void SizeClass::metabolisation(Nutrient& nutrient) {
  std::for_each(std::begin(alive_), std::end(alive_), [&](unsigned index) {
    heterotrophs_[index];

    double metabolicDeduction = heterotrophProcessor_.calculateMetabolicDeduction(heterotrophs_[index]);

    if ((heterotrophs_[index].getVolumeActual() - metabolicDeduction) > 0) {
      heterotrophs_[index].setHasFed(false);  // Reset for the next time step
      double waste = heterotrophs_[index].metabolise(metabolicDeduction);
      nutrient.addToVolume(waste);
    } else {
      starve(nutrient, index);
    }
  });
}

void SizeClass::starvation(Nutrient& nutrient) {
  sizeClassSubset([&](unsigned randomIndex) {
    Heterotroph& heterotroph = heterotrophs_[randomIndex];
    if (random_.getUniform() <= heterotrophProcessor_.calculateStarvationProbability(heterotroph)) {
      starve(nutrient, randomIndex);
    }
  });
}

void SizeClass::calculateFeedingProbability() {
    if(alive_.size() != 0) {
      double effectivePreyVolume = 0;
      double highestEffectiveSizeClassVolume = 0;
      unsigned coupledSizeClassIndex = 0;
      for(unsigned preyIndex = 0; preyIndex < numberOfSizeClasses_; ++preyIndex) {
        double effectiveSizeClassVolume = 0;


      }
    }


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

void SizeClass::sizeClassSubset(std::function<void(unsigned)> func) {
  std::size_t numberAlive = alive_.size();
  if (numberAlive != 0) {
    unsigned sizeClassSubset = heterotrophProcessor_.roundWithProbability(random_, numberAlive * sizeClassSubsetFraction_);
    for (auto _ = sizeClassSubset; _--;) {
      func(getRandomHeterotrophIndex());
    }
  }
}

void SizeClass::starve(Nutrient& nutrient, const unsigned index) {
  Heterotroph& heterotroph = removeHeterotroph(index);
  nutrient.addToVolume(heterotroph.getVolumeActual());
  heterotrophData_.incrementStarvedFrequencies(heterotroph.getSizeClassIndex());
}

size_t SizeClass::getPopulationSize() {
  return alive_.size();
}

unsigned SizeClass::getRandomHeterotrophIndex() {
  if(alive_.size() != 0) {
    unsigned randomIndex = random_.getUniformInt(alive_.size());
    return alive_[randomIndex];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& SizeClass::getRandomHeterotroph() {
  if(alive_.size() != 0) {
    unsigned randomIndex = random_.getUniformInt(alive_.size());
    return heterotrophs_[alive_[randomIndex]];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& SizeClass::getHeterotroph(const unsigned index) {
  if(alive_.size() != 0) {
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

const Heterotroph& SizeClass::getHeterotroph(const unsigned index) const {
    if(alive_.size() != 0) {
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

Heterotroph& SizeClass::removeHeterotroph(const unsigned index) {
  if(alive_.size() != 0) {
    alive_.erase(std::find(std::begin(alive_), std::end(alive_), index));
    dead_.push(index);
    return heterotrophs_[index];
  } else {
    throw std::runtime_error("Size class is empty...");
  }
}

void SizeClass::addHeterotroph(Heterotroph heterotroph) {
  if(alive_.size() != maxPopulation_) {
    unsigned index;
    if(dead_.size() == 0) {
      index = heterotrophs_.size();
      heterotrophs_.push_back(std::move(heterotroph));
    } else {
      index = dead_.front();
      dead_.pop();
      heterotrophs_[index] = std::move(heterotroph);
    }
    alive_.push_back(index);
  } else {
    throw std::runtime_error("Size class is full...");
  }
}
