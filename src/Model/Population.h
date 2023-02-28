#ifndef POPULATION
#define POPULATION

#include <vector>

#include "Autotrophs.h"
#include "Nutrient.h"
#include "HeterotrophProcessor.h"
#include "HeterotrophData.h"
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

  std::vector<SizeClass> sizeClasses_;
  HeterotrophProcessor heterotrophProcessor_;

  HeterotrophData heterotrophData_;
  RandomSimple random_;
};

#endif