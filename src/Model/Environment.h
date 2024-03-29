#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Life.h"
#include "Nutrient.h"

class Environment {
 public:
  Environment();

  void update();
  bool recordData();

  Nutrient& getNutrient();

  const Nutrient& getNutrient() const;

 private:
  Nutrient nutrient_;
  Life life_;
};

#endif
