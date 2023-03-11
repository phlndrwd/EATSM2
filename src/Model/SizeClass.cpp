#include "SizeClass.h"

#include <algorithm>
#include <climits>
#include <iostream>
#include <stdexcept>

#include <Parameters.h>
namespace {
Heterotroph heterotrophGenerator(double traitValue,
                                 double volume,
                                 unsigned sizeClassIndex) {
  std::vector<double> traitValues{traitValue};
  std::vector<bool> areTraitsMutant{false};
  Traits traits(traitValues, areTraitsMutant);
  Heterotroph heterotroph(std::move(traits), volume, sizeClassIndex);
  return heterotroph;
}
}  // anonymous namespace

SizeClass::SizeClass(Nutrient& nutrient,
                     Autotrophs& autotrophs,
                     const unsigned index,
                     const unsigned randomSeed,
                     bool runPopulate) :
    nutrient_(nutrient),
    autotrophs_(autotrophs),
    index_(index),
    sizeClassPreferences_(Parameters::Get()->getInterSizeClassPreferenceVector(index_)),
    sizeClassVolumes_(Parameters::Get()->getInterSizeClassVolumeVector(index_)),
    sizeClassMidPoint_(Parameters::Get()->getSizeClassMidPoint(index_)),
    sizeClassSubsetFraction_(Parameters::Get()->getSizeClassSubsetFraction()),
    maxPopulation_(Parameters::Get()->getMaximumSizeClassPopulation(index_)),
    autotrophSizeClassIndex_(Parameters::Get()->getAutotrophSizeClassIndex()),
    random_(randomSeed) {
  heterotrophs_.reserve(maxPopulation_);
  alive_.reserve(maxPopulation_);
  if(runPopulate == true) {
    populate();
  }
}

void SizeClass::populate() {
  double initialHeterotrophVolume = Parameters::Get()->getInitialHeterotrophVolume();
  double realInitialPopulationSize = initialHeterotrophVolume / sizeClassMidPoint_;
  unsigned initialPopulationSize = std::abs(realInitialPopulationSize);
  nutrient_.addToVolume(realInitialPopulationSize - initialPopulationSize);

  double traitValue = heterotrophProcessor_.volumeToTraitValue(sizeClassMidPoint_);
  std::generate_n(std::back_inserter(heterotrophs_), initialPopulationSize, [&] {
    return heterotrophGenerator(traitValue, sizeClassMidPoint_, index_);
  });
  std::cout << "Size class with index " << index_ << " initialised with " << initialPopulationSize <<
               " individuals." << std::endl;
}

std::vector<Heterotroph> SizeClass::update() {
  std::vector<Heterotroph> heterotrophsToMove;
  feeding();
  metabolisation();
  starvation();

  return heterotrophsToMove;
}

void SizeClass::feeding() {
//  sizeClassSubset([&](unsigned randomIndex) {
//    Heterotroph& heterotroph = heterotrophs_[randomIndex];
//    if (random_.getUniform() <= heterotrophProcessor_.calculateStarvationProbability(heterotroph)) {
//      starve(randomIndex);
//    }
//  });



//  if( sizeClassPopulation > 0 ) {
//      unsigned sizeClassPopulationSubset = mHeterotrophProcessor.RoundWithProbability( mRandom, sizeClassPopulation * mSizeClassSubsetFraction );

//    for( unsigned potentialEncounterIndex = 0; potentialEncounterIndex < sizeClassPopulationSubset; ++potentialEncounterIndex ) {
//      if( mRandom.GetUniform( ) <= mHeterotrophData.GetFeedingProbability( predatorIndex ) ) {
//        Types::HeterotrophPointer predator = GetRandomPredatorFromSizeClass( predatorIndex );
//        if( predator != nullptr ) {
//            unsigned coupledIndex = mHeterotrophData.GetCoupledSizeClassIndex( predatorIndex );
//            if( coupledIndex == mAutotrophSizeClassIndex ) FeedFromAutotrophs( predator );
//            else FeedFromHeterotrophs( predator, coupledIndex );
//        }
//      }
//    }
//  }
}

void SizeClass::metabolisation() {
  std::for_each(std::begin(alive_), std::end(alive_), [&](unsigned index) {
    heterotrophs_[index];

    double metabolicDeduction = heterotrophProcessor_.calculateMetabolicDeduction(heterotrophs_[index]);

    if ((heterotrophs_[index].getVolumeActual() - metabolicDeduction) > 0) {
      heterotrophs_[index].setHasFed(false);  // Reset for the next time step
      double waste = heterotrophs_[index].metabolise(metabolicDeduction);
      nutrient_.addToVolume(waste);
    } else {
      starve(index);
    }
  });
}

void SizeClass::starvation() {
  sizeClassSubset([&](unsigned randomIndex) {
    Heterotroph& heterotroph = heterotrophs_[randomIndex];
    if (random_.getUniform() <= heterotrophProcessor_.calculateStarvationProbability(heterotroph)) {
      starve(randomIndex);
    }
  });
}

void SizeClass::calculateFeedingProbability(std::vector<size_t>& populationSizes) {
  if(alive_.size() != 0) {
    effectivePreyVolume_ = 0;
    feedingProbabilty_ = 0;
    coupledSizeClassIndex_ = 0;

    double highestEffectiveSizeClassVolume = 0;
    unsigned preyIndex = 0;
    auto sizeClassVolumesIt = sizeClassVolumes_.begin();
    auto sizeClassPreferencesIt = sizeClassPreferences_.begin();
    std::for_each(begin(populationSizes), end(populationSizes),
    [&](size_t populationSize) {
      size_t effectivePopulationSize = preyIndex != index_ ? populationSize : populationSize - 1;
      double effectiveSizeClassVolume = *sizeClassVolumesIt * effectivePopulationSize;
      if(preyIndex == autotrophSizeClassIndex_) {
        effectiveSizeClassVolume += *sizeClassPreferencesIt * autotrophs_.getVolume();
      }
      if (effectiveSizeClassVolume > highestEffectiveSizeClassVolume) {
        highestEffectiveSizeClassVolume = effectiveSizeClassVolume;
        coupledSizeClassIndex_ = preyIndex;
      }
      effectivePreyVolume_ += effectiveSizeClassVolume;
      ++sizeClassPreferencesIt;
      ++sizeClassVolumesIt;
      ++preyIndex;
    });
    feedingProbabilty_ = heterotrophProcessor_.calculateFeedingProbability(index_, effectivePreyVolume_);
  }
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

void SizeClass::starve(const unsigned index) {
  Heterotroph& heterotroph = removeHeterotroph(index);
  nutrient_.addToVolume(heterotroph.getVolumeActual());
  //heterotrophData_.incrementStarvedFrequencies(heterotroph.getSizeClassIndex());
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
    std::vector<Heterotroph>::iterator heterotrophsIt;
    if(dead_.size() != 0) {
      index = dead_.front();
      dead_.pop();
      heterotrophsIt = heterotrophs_.begin();
      std::advance(heterotrophsIt, index);
    } else {
      index = heterotrophs_.size();
      heterotrophsIt = heterotrophs_.end();
    }
    heterotrophs_.insert(heterotrophsIt, std::move(heterotroph));
    alive_.push_back(index);
  } else {
    throw std::runtime_error("Size class is full...");
  }
}
