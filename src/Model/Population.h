#ifndef POPULATION
#define POPULATION

#include <vector>

#include "Autotrophs.h"
#include "Nutrient.h"
#include "HeterotrophProcessor.h"
#include "RandomSimple.h"
#include "SizeClass.h"

class Population {
 public:
  Population() = delete;
  Population(Nutrient&, Autotrophs&, unsigned);

  void update();
  bool recordData();

 private:
  void createInitialPopulation();

  Nutrient& nutrient_;
  Autotrophs& autotrophs_;
  const unsigned numberOfSizeClasses_;

  std::vector<SizeClass> sizeClasses_;
  HeterotrophProcessor heterotrophProcessor_;
  RandomSimple random_;
};

#endif
