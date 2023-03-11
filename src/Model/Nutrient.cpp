#include "Nutrient.h"

#include <iostream>

#include "DataRecorder.h"

Nutrient::Nutrient() {
  volume_ = 0;
  std::cout << "Nutrient pool created." << std::endl;
}

void Nutrient::recordData() {
  DataRecorder::get()->addDataTo("NutrientVolume", volume_);
  DataRecorder::get()->addDataTo("ToNutrientFlux", toFlux_);
  toFlux_ = 0;
}

const double& Nutrient::getVolume() const {
  return volume_;
}

void Nutrient::addToVolume(const double volume) {
  volume_ += volume;
  toFlux_ += volume;
}

void Nutrient::subtractFromVolume(const double volume) { volume_ -= volume; }
