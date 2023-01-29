#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Autotrophs.h"
#include "Heterotrophs.h"
#include "Nutrient.h"

class Environment {
 public:
  Environment();
  ~Environment();

  void update();
  bool recordData();

  Nutrient& getNutrient();
  Autotrophs& getAutotrophs();
  Heterotrophs& getHeterotrophs();

 private:
  Nutrient nutrient_;
  Autotrophs autotrophs_;
  Heterotrophs heterotrophs_;
};

#endif
