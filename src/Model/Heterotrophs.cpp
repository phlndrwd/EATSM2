#include "Heterotrophs.h"

#include <algorithm>
#include <iostream>
#include <climits>

#include "Parameters.h"

namespace {
SizeClass sizeClassGenerator(Nutrient& nutrient,
                             Autotrophs& autotrophs,
                             RandomSimple& random,
                             unsigned& index,
                             bool runPopulate) {
  SizeClass sizeClass(nutrient, autotrophs, index, random.getUniformInt(1, UINT_MAX), runPopulate);
  ++index;
  return sizeClass;
}
}  // anonymous namespace

Heterotrophs::Heterotrophs(Nutrient& nutrient,
                           Autotrophs& autotrophs,
                           const unsigned numberOfSizeClasses) :
    nutrient_(nutrient),
    autotrophs_(autotrophs),
    numberOfSizeClasses_(numberOfSizeClasses),
    random_(Parameters::Get()->getRandomSeed()) {

  double initialIdealVolume = Parameters::Get()->getSmallestIndividualVolume() *
                              Parameters::Get()->getPreferredPreyVolumeRatio();
  unsigned indexToPopulate = heterotrophProcessor_.findSizeClassIndexFromVolume(initialIdealVolume);
  unsigned index = 0;
  std::generate_n(std::back_inserter(sizeClasses_), numberOfSizeClasses_, [&] {
    return sizeClassGenerator(nutrient_, autotrophs_, random_, index, indexToPopulate == index);
  } );
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
    std::vector<Heterotroph> heterotrophsToMove = sizeClass.update();
    for (const auto& heterotroph : heterotrophsToMove) {
      std::vector<SizeClass>::iterator sizeClassIt = sizeClasses_.begin();
      std::advance(sizeClassIt, heterotroph.getSizeClassIndex());
      sizeClassIt->addHeterotroph(heterotroph);
    }
  });
}
