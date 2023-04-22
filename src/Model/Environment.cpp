#include "Environment.h"

#include<iostream>

#include "Parameters.h"

Environment::Environment() :
    nutrient_(),
    heterotrophs_(nutrient_, Parameters::Get()->getNumberOfSizeClasses()) {
  std::cout << "Environment created." << std::endl << std::endl;
}

void Environment::update() {
  heterotrophs_.update();
}

bool Environment::recordData() {
  //bool isAlive = population_.recordData();
  nutrient_.recordData();

  return true;
}

Nutrient& Environment::getNutrient() {
  return nutrient_;
}

const Nutrient& Environment::getNutrient() const {
  return nutrient_;
}
