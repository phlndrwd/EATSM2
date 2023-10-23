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

Heterotrophs::Heterotrophs(Nutrient& nutrient) :
    nutrient_(nutrient),
    sizeClassBoundaries_(Parameters::Get()->getSizeClassBoundaries()),
    numberOfSizeClasses_(Parameters::Get()->getNumberOfSizeClasses()),
    random_(Parameters::Get()->getRandomSeed()),  // Is this the first time random is used?
    encounterAlgorithm_(random_.getUniformInt(1, UINT_MAX)) {
  unsigned autotrophIndex = Parameters::Get()->getAutotrophSizeClassIndex();
  double idealInitialVolume = Parameters::Get()->getSmallestIndividualVolume() *
                              Parameters::Get()->getPreferredPreyVolumeRatio();
  unsigned heterotrophIndex = findSizeClassIndexFromVolume(idealInitialVolume);
  unsigned index = 0;
  std::generate_n(std::back_inserter(sizeClasses_), numberOfSizeClasses_, [&] {
    double initialAutotrophVolume = autotrophIndex != index ? 0 : Parameters::Get()->getInitialAutotrophVolume();
    double initialHeterotrophVolume = heterotrophIndex != index ? 0 : Parameters::Get()->getInitialHeterotrophVolume();
    return sizeClassGenerator(nutrient_, random_, initialAutotrophVolume, initialHeterotrophVolume, index);
  });
}

void Heterotrophs::update() {
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_), [&](SizeClass& sizeClass) {
    std::vector<structs::MovingHeterotroph> movingHeterotrophs;
    encounterAlgorithm_.update(sizeClasses_, sizeClass);
    sizeClass.update(movingHeterotrophs);

    for (const auto& movingHeterotroph : movingHeterotrophs) {
      // PJU FIX - Determine SizeClassIndex here.

      if (movingHeterotroph.direction_ == enums::eMoveDown) {
        // Search down from current sizeClass
        auto sizeClassDownIt = sizeClasses_.rbegin();
        std::advance(sizeClassDownIt, numberOfSizeClasses_ + sizeClass.getIndex() + 1);
        std::for_each(sizeClassDownIt, sizeClasses_.rend(), [&](SizeClass& nextSizeClass){
          //
        });




      } else if(movingHeterotroph.direction_ == enums::eMoveUp) {
        Heterotroph& heterotroph = movingHeterotroph.heterotroph_;
        std::vector<SizeClass>::iterator destSizeClass =
        std::find_if(sizeClasses_.begin(), sizeClasses_.end(), [&](SizeClass& nextSizeClass) {
          if(heterotroph.getVolumeActual() >= sizeClassBoundaries_[nextSizeClass.getIndex()]) {
            return true;
          } else {
            return false;
          }
        });
        destSizeClass = std::prev(destSizeClass);
        destSizeClass->addHeterotroph(heterotroph);

        //// Search up from current sizeClass
        //auto sizeClassUpIt = sizeClasses_.begin();
        //std::advance(sizeClassUpIt, sizeClass.getIndex());
        //std::for_each(sizeClassUpIt, sizeClasses_.end(), [&](SizeClass& nextSizeClass){
        //  //
        //});
      }

      //std::advance(sizeClassIt, heterotroph.getSizeClassIndex());
      //sizeClassIt->addHeterotroph(movingHeterotroph.heterotroph_);
    }
  });
}

unsigned Heterotrophs::findSizeClassIndexFromVolume(const double volume) const {
  unsigned sizeClassIndex = 0;

  for (unsigned index = 1; index <= numberOfSizeClasses_; ++index) {
    if (volume < sizeClassBoundaries_[index]) {
      sizeClassIndex = index - 1;
      break;
    }
  }
  return sizeClassIndex;
}
