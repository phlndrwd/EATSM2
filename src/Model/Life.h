#ifndef POPULATION
#define POPULATION

#include <vector>

#include "EncounterAlgorithm.h"
#include "Nutrient.h"
#include "RandomSimple.h"
#include "SizeClass.h"

class Life {
 public:
  Life() = delete;
   explicit Life(Nutrient&);

  void update();

 private:
  unsigned findSizeClassIndexFromVolume(const double) const;

  Nutrient& nutrient_;

  const std::vector<double> sizeClassBoundaries_;
  const unsigned numberOfSizeClasses_;

  RandomSimple random_;
  EncounterAlgorithm encounterAlgorithm_;
  std::vector<SizeClass> sizeClasses_;
};

#endif