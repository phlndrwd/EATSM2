#include "Autotrophs.h"

#include <iostream>

#include "DataRecorder.h"
#include "Nutrient.h"

Autotrophs::Autotrophs(Nutrient& nutrient, const double initialVolume) :
    nutrient_(nutrient), volume_(initialVolume) {
  //std::cout << "Autotroph pool created." << std::endl;
}

void Autotrophs::update() {
  double growthVolume = nutrient_.getVolume();
  addToVolume(growthVolume);
  nutrient_.subtractFromVolume(growthVolume);
}

void Autotrophs::addToVolume(const double volume) {
  volume_ += volume;
  toFlux_ += volume;
}

void Autotrophs::subtractFromVolume(const double volume) {
  volume_ -= volume;
}

void Autotrophs::recordData() {
  DataRecorder::get()->addDataTo("AutotrophVolume", volume_);
  DataRecorder::get()->addDataTo("ToAutotrophFlux", toFlux_);
  toFlux_ = 0;
}

double& Autotrophs::getVolume() {
  return volume_;
}

const double& Autotrophs::getVolume() const {
  return volume_;
}
