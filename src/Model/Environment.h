#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Autotrophs.h"
#include "Heterotrophs.h"
#include "Nutrient.h"
#include "Population.h"

class Environment {
 public:
  Environment();

  void update();
  bool recordData();

  Nutrient& getNutrient();
  Autotrophs& getAutotrophs();

  const Nutrient& getNutrient() const;
  const Autotrophs& getAutotrophs() const;

 private:
  Nutrient nutrient_;
  Autotrophs autotrophs_;
  Population population_;
};

#endif
