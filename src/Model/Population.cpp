#include "Population.h"

#include <algorithm>

#include "Parameters.h"

Population::Population(Nutrient& nutrient, Autotrophs& autotrophs, unsigned numberOfSizeClasses) :
    nutrient_(nutrient),
    autotrophs_(autotrophs),
    numberOfSizeClasses_(numberOfSizeClasses),
    random_(Parameters::Get()->getRandomSeed()) {
  for(unsigned index = 0; index < numberOfSizeClasses_; ++index) {
    sizeClasses_.push_back(SizeClass(random_,
                                     Parameters::Get()->getSizeClassMidPoint(index),
                                     Parameters::Get()->getMaximumSizeClassPopulation(index)));
  }
}

void Population::update() {
  std::for_each(std::begin(sizeClasses_), std::end(sizeClasses_),
  [] (SizeClass& sizeClass)
  {
    sizeClass.update();
  });
}

bool Population::recordData() {
  return true;
}
