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
  Population(Nutrient&, Autotrophs&, const unsigned);

  void update();
  bool recordData();

 private:
  void createInitialPopulation();
  void calculateFeedingProbabilities();

  Nutrient& nutrient_;
  Autotrophs& autotrophs_;

  std::vector<SizeClass> sizeClasses_;
  HeterotrophProcessor heterotrophProcessor_;

  HeterotrophData heterotrophData_;
  RandomSimple random_;
};

#endif
