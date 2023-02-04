#include "Environment.h"

#include<iostream>

Environment::Environment() :
    nutrient_(),
    autotrophs_(nutrient_),
    heterotrophs_(nutrient_, autotrophs_) {
  std::cout << "Environment created." << std::endl << std::endl;
}

void Environment::update() {
  autotrophs_.update();
  heterotrophs_.update();
}

bool Environment::recordData() {
  bool isAlive = heterotrophs_.recordData();
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

Heterotrophs& Environment::getHeterotrophs() {
  return heterotrophs_;
}

const Nutrient& Environment::getNutrient() const {
  return nutrient_;
}

const Autotrophs& Environment::getAutotrophs() const {
  return autotrophs_;
}

const Heterotrophs& Environment::getHeterotrophs() const {
  return heterotrophs_;
}

