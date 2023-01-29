#include "Nutrient.h"

#include <iostream>

#include "DataRecorder.h"
#include "InitialState.h"

Nutrient::Nutrient() {
  if (InitialState::Get()->isInitialised() == true)
    volume_ = InitialState::Get()->getNutrientVolume();
  else
    volume_ = 0;

  std::cout << "Nutrient pool created." << std::endl;
}

Nutrient::~Nutrient() {}

void Nutrient::recordData() {
  DataRecorder::get()->addDataTo("NutrientVolume", volume_);
  DataRecorder::get()->addDataTo("ToNutrientFlux", toFlux_);
  toFlux_ = 0;
}

double Nutrient::getVolume() { return volume_; }

void Nutrient::setVolume(const double volume) { volume_ = volume; }

void Nutrient::addToVolume(const double volume) {
  volume_ += volume;
  toFlux_ += volume;
}

void Nutrient::subtractFromVolume(const double volume) { volume_ -= volume; }
