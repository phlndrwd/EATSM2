#ifndef POPULATION
#define POPULATION

#include <memory>
#include <vector>

#include "Nutrient.h"
#include "HeterotrophProcessor.h"
#include "RandomSimple.h"
#include "SizeClass.h"

class Heterotrophs {
 public:
  Heterotrophs() = delete;
  explicit Heterotrophs(Nutrient&, const unsigned);

  void update();

 private:
  Nutrient& nutrient_;
  const unsigned numberOfSizeClasses_;

  RandomSimple random_;
  std::vector<SizeClass> sizeClasses_;
  HeterotrophProcessor heterotrophProcessor_;
};

#endif
