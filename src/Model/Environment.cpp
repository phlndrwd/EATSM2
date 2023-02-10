#include "Environment.h"

#include<iostream>

#include "Parameters.h"

Environment::Environment() :
    nutrient_(),
    autotrophs_(nutrient_),
    population_(nutrient_, autotrophs_, Parameters::Get()->getNumberOfSizeClasses()) {
  std::cout << "Environment created." << std::endl << std::endl;
}

void Environment::update() {
  autotrophs_.update();
  population_.update();
}

bool Environment::recordData() {
  bool isAlive = population_.recordData();
  autotrophs_.recordData();
  nutrient_.recordData();

  return isAlive;
}

Nutrient& Environment::getNutrient() {
  return nutrient_;
}

Autotrophs& Environment::getAutotrophs() {
  return autotrophs_;
}

const Nutrient& Environment::getNutrient() const {
  return nutrient_;
}

const Autotrophs& Environment::getAutotrophs() const {
  return autotrophs_;
}

