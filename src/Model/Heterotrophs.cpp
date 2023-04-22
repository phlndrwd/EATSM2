#include "Heterotrophs.h"

#include <algorithm>
#include <iostream>
#include <climits>

#include "Parameters.h"

namespace {
SizeClass sizeClassGenerator(std::vector<SizeClass>& sizeClasses,
                             Nutrient& nutrient,
                             RandomSimple& random,
                             const double initialAutotrophVolume,
                             const double initialHeterotrophVolume,
                             unsigned& index) {
  SizeClass sizeClass(sizeClasses, nutrient, initialAutotrophVolume, initialHeterotrophVolume, index, random.getUniformInt(1, UINT_MAX));
  ++index;
  return sizeClass;
}
}  // anonymous namespace

Heterotrophs::Heterotrophs(Nutrient& nutrient,
                           const unsigned numberOfSizeClasses) :
    nutrient_(nutrient),
    numberOfSizeClasses_(numberOfSizeClasses),
    random_(Parameters::Get()->getRandomSeed()) {
  unsigned autotrophIndex = Parameters::Get()->getAutotrophSizeClassIndex();
  double idealInitialVolume = Parameters::Get()->getSmallestIndividualVolume() *
                              Parameters::Get()->getPreferredPreyVolumeRatio();
  unsigned heterotrophIndex = heterotrophProcessor_.findSizeClassIndexFromVolume(idealInitialVolume);
  unsigned index = 0;
  std::generate_n(std::back_inserter(sizeClasses_), numberOfSizeClasses_, [&] {
    double initialAutotrophVolume = autotrophIndex != index ? 0 : Parameters::Get()->getInitialAutotrophVolume();
    double initialHeterotrophVolume = heterotrophIndex != index ? 0 : Parameters::Get()->getInitialHeterotrophVolume();
    std::cout << "Size class " << index << ", initialAutotrophVolume " << initialAutotrophVolume << ", initialHeterotrophVolume " << initialHeterotrophVolume << std::endl;
    return sizeClassGenerator(sizeClasses_, nutrient_, random_, initialAutotrophVolume, initialHeterotrophVolume, index);
  });
}

void Heterotrophs::update() {
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_),
  [&](SizeClass& sizeClass) {
    std::vector<structs::MovingHeterotroph> movingHeterotrophs = sizeClass.update();
    for (const auto& movingHeterotroph : movingHeterotrophs) {
      std::vector<SizeClass>::iterator sizeClassIt = sizeClasses_.begin();

      // PJU FIX - Determine SizeClassIndex here.

      //std::advance(sizeClassIt, heterotroph.getSizeClassIndex());
      sizeClassIt->addHeterotroph(movingHeterotroph.heterotroph);
    }
  });
}
