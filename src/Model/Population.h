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
  Population(Nutrient&, Autotrophs&, const unsigned);

  void update();
  bool recordData();

 private:
  void createInitialPopulation();
  void calculateFeedingProbabilities();

  Nutrient& nutrient_;
  Autotrophs& autotrophs_;

  const unsigned numberOfSizeClasses_;

  RandomSimple random_;
  std::vector<SizeClass> sizeClasses_;
  HeterotrophProcessor heterotrophProcessor_;
};

#endif
