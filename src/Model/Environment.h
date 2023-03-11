#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Autotrophs.h"
#include "Nutrient.h"
#include "Heterotrophs.h"

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
  Heterotrophs population_;
};

#endif
