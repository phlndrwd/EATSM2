#ifndef POPULATION
#define POPULATION

#include <vector>

#include "Autotrophs.h"
#include "Nutrient.h"
#include "RandomSimple.h"
#include "SizeClass.h"

class Population {
 public:
  Population() = delete;
  Population(Nutrient&, Autotrophs&, unsigned);

  void update();

 private:
  Nutrient& nutrient_;
  Autotrophs& autotrophs_;
  const unsigned numberOfSizeClasses_;

  std::vector<SizeClass> sizeClasses_;
  RandomSimple random_;
};

#endif
