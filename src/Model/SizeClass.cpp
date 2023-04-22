#include "SizeClass.h"

#include <algorithm>
#include <climits>
#include <iostream>
#include <stdexcept>

#include "Parameters.h"

namespace {
Heterotroph heterotrophGenerator(double traitValue,
                                 double volume) {
  std::vector<double> traitValues{traitValue};
  std::vector<bool> areTraitsMutant{false};
  Traits traits(traitValues, areTraitsMutant);
  Heterotroph heterotroph(std::move(traits), volume);
  return heterotroph;
}
}  // anonymous namespace

SizeClass::SizeClass(Nutrient& nutrient,
                     const double initialAutotrophVolume,
                     const double initialHeterotrophVolume,
                     const unsigned index,
                     const unsigned randomSeed) :
    nutrient_(nutrient),
    index_(index),
    sizeClassPreferences_(Parameters::Get()->getInterSizeClassPreferenceVector(index_)),
    sizeClassVolumes_(Parameters::Get()->getInterSizeClassVolumeVector(index_)),
    sizeClassMidPoint_(Parameters::Get()->getSizeClassMidPoint(index_)),
    sizeClassSubsetFraction_(Parameters::Get()->getSizeClassSubsetFraction()),
    numberOfSizeClasses_(Parameters::Get()->getNumberOfSizeClasses()),
    maxPopulation_(Parameters::Get()->getMaximumSizeClassPopulation(index_)),
    autotrophs_(nutrient, initialAutotrophVolume),
    random_(randomSeed) {
  heterotrophs_.reserve(maxPopulation_);
  alive_.reserve(maxPopulation_);
  populate(initialHeterotrophVolume);
  //std::advance(autotrophSizeClassIt_, Parameters::Get()->getAutotrophSizeClassIndex());
}

void SizeClass::populate(const double volumeToInitialise) {
  if(volumeToInitialise > 0) {
    double realInitialPopulationSize = volumeToInitialise / sizeClassMidPoint_;
    unsigned initialPopulationSize = std::abs(realInitialPopulationSize);
    nutrient_.addToVolume(realInitialPopulationSize - initialPopulationSize);

    double traitValue = heterotrophProcessor_.volumeToTraitValue(sizeClassMidPoint_);
    unsigned heterotrophIndex = 0;
    std::generate_n(std::back_inserter(heterotrophs_), initialPopulationSize, [&] {
      alive_.push_back(heterotrophIndex);
      ++heterotrophIndex;
      return heterotrophGenerator(traitValue, sizeClassMidPoint_);
    });
    std::cout << "Size class with index " << index_ << " initialised with " << initialPopulationSize <<
                 " heterotrophs." << std::endl;
  }
}

std::vector<structs::MovingHeterotroph> SizeClass::update(std::vector<SizeClass>& sizeClasses) {
  std::vector<structs::MovingHeterotroph> movingHeterotrophs;
  feeding(sizeClasses);
  metabolisation();
  starvation();

  return movingHeterotrophs;
}

void SizeClass::feeding(std::vector<SizeClass>& sizeClasses) {
  std::vector<SizeClass>::iterator coupledSizeClassIt = calcFeedingProbability(sizeClasses);
  sizeClassSubset([&](unsigned randomIndex) {
    Heterotroph& predator = heterotrophs_[randomIndex];

    if(feedingStrategy_ == enums::eCarnivore) {
      feedFromHeterotrophs(predator, coupledSizeClassIt);
    } else if(feedingStrategy_ == enums::eHerbivore){
      feedFromAutotrophs(predator);
    }
  });
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

std::vector<size_t> SizeClass::getPopulationSizes(std::vector<SizeClass>& sizeClasses) {
  std::vector<size_t> populationSizes(numberOfSizeClasses_, 0);
//  auto popSizesIt = populationSizes.begin();
//  std::for_each(std::begin(sizeClasses), std::end(sizeClasses),
//  [&](SizeClass& sizeClass) {
//    *popSizesIt = sizeClass.getPopulationSize();
//    std::cout << "*popSizesIt " << *popSizesIt << std::endl;
//    if(this == &sizeClass) {
//      *popSizesIt--;
//    }
//    std::advance(popSizesIt, 1);
//  });

  for(unsigned i = 0; i < numberOfSizeClasses_; ++i) {
    populationSizes[i] = sizeClasses[i].getPopulationSize();
    if(this == &sizeClasses[i]) {
      populationSizes[i]--;
    }
  }
  return populationSizes;
}

std::vector<SizeClass>::iterator SizeClass::calcFeedingProbability(std::vector<SizeClass>& sizeClasses) {
  std::vector<SizeClass>::iterator coupledSizeClassIt = sizeClasses.begin();
  if(alive_.size() != 0) {
    std::vector<double> effectiveSizeClassVolumes(numberOfSizeClasses_, 0);
    calcEffectiveSizeClassVolumes(sizeClasses, effectiveSizeClassVolumes);
    coupledSizeClassIt = setCoupledSizeClass(effectiveSizeClassVolumes, sizeClasses);
    calcFeedingStrategy();
    feedingProbabilty_ = heterotrophProcessor_.calcFeedingProbability(index_, effectivePreyVolume_);
  }
  return coupledSizeClassIt;
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

void SizeClass::calcEffectiveSizeClassVolumes(std::vector<SizeClass>& sizeClasses,
                                              std::vector<double>& effectiveSizeClassVolumes) {
  effectivePreyVolume_ = 0;
  effectiveAutotrophVolume_ = 0;
  feedingProbabilty_ = 0;

  std::vector<size_t> populationSizes = getPopulationSizes(sizeClasses);
//  auto sizeClassVolumesIt = sizeClassVolumes_.begin();
//  auto sizeClassPreferencesIt = sizeClassPreferences_.begin();
//  auto effectiveSizeClassVolumesIt = effectiveSizeClassVolumes.begin();
//  // Calculate effective prey volumes
//  std::for_each(begin(populationSizes), end(populationSizes),
//  [&](size_t populationSize) {

//    *effectiveSizeClassVolumesIt = *sizeClassVolumesIt * populationSize;
//    effectiveAutotrophVolume_ = *sizeClassPreferencesIt * autotrophs_.getVolume();

//    *effectiveSizeClassVolumesIt += effectiveAutotrophVolume_;
//    effectivePreyVolume_ += *effectiveSizeClassVolumesIt;

//    std::advance(effectiveSizeClassVolumesIt, 1);
//    std::advance(sizeClassPreferencesIt, 1);
//    std::advance(sizeClassVolumesIt, 1);
//  });

  for(unsigned i = 0; i < numberOfSizeClasses_; ++i) {
    double effectiveSizeClassAutotrophVolume = sizeClassPreferences_[i] * sizeClasses[i].getAutotrophs().getVolume();
    effectiveAutotrophVolume_ += effectiveSizeClassAutotrophVolume;
    effectiveSizeClassVolumes[i] = (sizeClassVolumes_[i] * populationSizes[i]) + effectiveSizeClassAutotrophVolume;

    effectivePreyVolume_ += effectiveSizeClassVolumes[i];
  }
}


  std::vector<SizeClass>::iterator SizeClass::setCoupledSizeClass(const std::vector<double>& effectiveSizeClassVolumes,
                                                                  std::vector<SizeClass>& sizeClasses) {
  // Default to largest populated size class
  std::vector<SizeClass>::iterator coupledSizeClassIt = std::prev(sizeClasses.end());
  double randEffectivePreyValue = random_.getUniform() * effectivePreyVolume_;
  double effectivePreySum = 0;
  unsigned index = 0;
  // Return from find_if is not used - used here to mimic break statement in classic for
  std::find_if(begin(effectiveSizeClassVolumes), end(effectiveSizeClassVolumes),
    [&](double effectiveSizeClassVolume) {
    effectivePreySum += effectiveSizeClassVolume;
    if(effectivePreySum >= randEffectivePreyValue) {
      std::advance(coupledSizeClassIt, -(numberOfSizeClasses_ - 1) + index);
      return true;
    } else {
      ++index;
      return false;
    }
  });
  return coupledSizeClassIt;
}

void SizeClass::calcFeedingStrategy() {
  feedingStrategy_ = enums::eCarnivore;
  double probHerbivory = effectiveAutotrophVolume_ / effectivePreyVolume_;
  if(random_.getUniform() <= probHerbivory) {
    feedingStrategy_ = enums::eHerbivore;
  }
}

void SizeClass::feedFromHeterotrophs(Heterotroph& predator, std::vector<SizeClass>::iterator& coupledSizeClassIt) {
    if(coupledSizeClassIt->getPopulationSize() != 0) {
      Heterotroph& prey = coupledSizeClassIt->getRandomHeterotroph();
      while(&predator == &prey) { // Predators cannot eat themselves
        prey = coupledSizeClassIt->getRandomHeterotroph();
      }
    }
//  Types::HeterotrophPointer prey = GetRandomPreyFromSizeClass(coupledIndex, predator);
//  if(prey != nullptr) {
//    double preyVolume = prey->GetVolumeActual();
//    mHeterotrophData.IncrementCarnivoreFrequencies(predator, prey);

//    double waste = predator->ConsumePreyVolume(preyVolume);
//    ++mFedCount[predator->GetSizeClassIndex()];

//    mHeterotrophProcessor.UpdateCarnivoreTrophicIndex(predator, prey);
//    mNutrient.AddToVolume(waste);
//    Kill(prey);
//  }
}

void SizeClass::feedFromAutotrophs(Heterotroph& grazer) {
//  if(mAutotrophs.GetVolume( ) > mSmallestIndividualVolume) {
//    mAutotrophs.SubtractFromVolume(mSmallestIndividualVolume);
//    mHeterotrophData.IncrementVegetarianFrequencies(grazer);

//    double waste = grazer->ConsumePreyVolume(mSmallestIndividualVolume);
//    ++mFedCount[grazer->GetSizeClassIndex()];

//    mHeterotrophProcessor.UpdateHerbivoreTrophicIndex(grazer);
//    mNutrient.AddToVolume(waste);
//  }
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

Autotrophs& SizeClass::getAutotrophs() {
  return autotrophs_;
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
