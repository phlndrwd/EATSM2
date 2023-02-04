#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Autotrophs.h"
#include "Heterotrophs.h"
#include "Nutrient.h"

class Environment {
 public:
  Environment();

  void update();
  bool recordData();

  Nutrient& getNutrient();
  Autotrophs& getAutotrophs();
  Heterotrophs& getHeterotrophs();

  const Nutrient& getNutrient() const;
  const Autotrophs& getAutotrophs() const;
  const Heterotrophs& getHeterotrophs() const;

 private:
  Nutrient nutrient_;
  Autotrophs autotrophs_;
  Heterotrophs heterotrophs_;
};

#endif
