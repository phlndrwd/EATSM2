#include "Heterotrophs.h"

#include <algorithm>
#include <iostream>
#include <climits>

#include "Parameters.h"

namespace {
SizeClass sizeClassGenerator(Nutrient& nutrient,
                             RandomSimple& random,
                             const double initialAutotrophVolume,
                             const double initialHeterotrophVolume,
                             unsigned& index) {
  SizeClass sizeClass(nutrient, initialAutotrophVolume, initialHeterotrophVolume, index, random.getUniformInt(1, UINT_MAX));
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
    return std::make_unique<SizeClass>(sizeClassGenerator(nutrient_, random_, initialAutotrophVolume, initialHeterotrophVolume, index));
  });
}

void Heterotrophs::update() {
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_),
  [&](std::unique_ptr<SizeClass>& sizeClass) {
    std::vector<structs::MovingHeterotroph> movingHeterotrophs = sizeClass.get()->update(sizeClasses_);
    for (const auto& movingHeterotroph : movingHeterotrophs) {
      std::vector<std::unique_ptr<SizeClass>>::iterator sizeClassIt = sizeClasses_.begin();

      // PJU FIX - Determine SizeClassIndex here.

      //std::advance(sizeClassIt, heterotroph.getSizeClassIndex());
      sizeClassIt->get()->addHeterotroph(movingHeterotroph.heterotroph);
    }
  });
}
