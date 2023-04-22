#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Nutrient.h"
#include "Heterotrophs.h"

class Environment {
 public:
  Environment();

  void update();
  bool recordData();

  Nutrient& getNutrient();

  const Nutrient& getNutrient() const;

 private:
  Nutrient nutrient_;
  Heterotrophs heterotrophs_;
};

#endif
