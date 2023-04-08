#ifndef POPULATION
#define POPULATION

#include <vector>

#include "Autotrophs.h"
#include "Nutrient.h"
#include "HeterotrophProcessor.h"
#include "RandomSimple.h"
#include "SizeClass.h"

class Heterotrophs {
 public:
  Heterotrophs() = delete;
  Heterotrophs(Nutrient&, Autotrophs&, const unsigned);

  void update();

 private:
  Nutrient& nutrient_;
  Autotrophs& autotrophs_;

  const unsigned numberOfSizeClasses_;

  RandomSimple random_;
  std::vector<SizeClass> sizeClasses_;
  HeterotrophProcessor heterotrophProcessor_;
};

#endif
