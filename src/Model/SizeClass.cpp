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
                     Autotrophs& autotrophs,
                     const double volumeToInitialise,
                     const unsigned index,
                     const unsigned randomSeed) :
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
  populate(volumeToInitialise);
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

std::vector<structs::MovingHeterotroph> SizeClass::update(std::vector<size_t>& populationSizes) {
  std::vector<structs::MovingHeterotroph> movingHeterotrophs;
  feeding(populationSizes);
  metabolisation();
  starvation();

  return movingHeterotrophs;
}

void SizeClass::feeding(std::vector<size_t>& populationSizes) {
  calcFeedingProbability(populationSizes);
  sizeClassSubset([&](unsigned randomIndex) {
    Heterotroph& predator = heterotrophs_[randomIndex];

    if(feedingStrategy_ == enums::eCarnivore) {
      feedFromHeterotrophs(predator);
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

void SizeClass::calcFeedingProbability(std::vector<size_t>& populationSizes) {
  if(alive_.size() != 0) {
    std::vector<double> effectiveSizeClassVolumes(populationSizes.size(), 0);
    calcEffectiveSizeClassVolumes(populationSizes, effectiveSizeClassVolumes);
    calcCoupledSizeClassIndex(effectiveSizeClassVolumes);
    calcFeedingStrategy();
    feedingProbabilty_ = heterotrophProcessor_.calcFeedingProbability(index_, effectivePreyVolume_);
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

void SizeClass::calcEffectiveSizeClassVolumes(std::vector<size_t>& populationSizes,
                                              std::vector<double>& effectiveSizeClassVolumes) {
  effectivePreyVolume_ = 0;
  feedingProbabilty_ = 0;
  coupledSizeClassIndex_ = 0;

  unsigned preyIndex = 0;
  auto sizeClassVolumesIt = sizeClassVolumes_.begin();
  auto sizeClassPreferencesIt = sizeClassPreferences_.begin();
  auto effectiveSizeClassVolumesIt = effectiveSizeClassVolumes.begin();
  // Calculate effective prey volumes
  std::for_each(begin(populationSizes), end(populationSizes),
  [&](size_t populationSize) {
    size_t effectivePopulationSize = preyIndex != index_ ? populationSize : populationSize - 1;

    *effectiveSizeClassVolumesIt = *sizeClassVolumesIt * effectivePopulationSize;
    if(preyIndex == autotrophSizeClassIndex_) {
      effectiveAutotrophVolume_ = *sizeClassPreferencesIt * autotrophs_.getVolume();
      *effectiveSizeClassVolumesIt += effectiveAutotrophVolume_;
    }
    effectivePreyVolume_ += *effectiveSizeClassVolumesIt;

    ++effectiveSizeClassVolumesIt;
    ++sizeClassPreferencesIt;
    ++sizeClassVolumesIt;
    ++preyIndex;
  });
}

void SizeClass::calcCoupledSizeClassIndex(std::vector<double>& effectiveSizeClassVolumes) {
  double randEffectivePreyValue = random_.getUniform() * effectivePreyVolume_;
  double effectivePreySum = 0;
  unsigned numberOfSizeClasses = effectiveSizeClassVolumes.size();
  for(unsigned index = 0; index < numberOfSizeClasses; ++index) {
    effectivePreySum += effectiveSizeClassVolumes[index];
    if(effectivePreySum >= randEffectivePreyValue) {
      coupledSizeClassIndex_ = index;
      break;
    }
  }
  coupledSizeClassIndex_ = numberOfSizeClasses;
}

void SizeClass::calcFeedingStrategy() {
  feedingStrategy_ = enums::eCarnivore;
  if(coupledSizeClassIndex_ == autotrophSizeClassIndex_) {
    double probHerbivory = effectiveAutotrophVolume_ / effectivePreyVolume_;
    if(random_.getUniform() <= probHerbivory) {
      feedingStrategy_ = enums::eHerbivore;
    }
  }
}

void SizeClass::feedFromHeterotrophs(Heterotroph& predator) {
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
