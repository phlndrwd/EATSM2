#include "Autotrophs.h"

#include <iostream>

#include "DataRecorder.h"
#include "InitialState.h"
#include "Nutrient.h"
#include "Parameters.h"

Autotrophs::Autotrophs(Nutrient& nutrient) : nutrient_(nutrient) {
  if (InitialState::Get()->isInitialised() == true)
    volume_ = InitialState::Get()->getAutotrophVolume();
  else
    volume_ = Parameters::Get()->getInitialAutotrophVolume();

  std::cout << "Autotroph pool created." << std::endl;
}

Autotrophs::~Autotrophs() {}

void Autotrophs::recordData() {
  DataRecorder::get()->addDataTo("AutotrophVolume", volume_);
  DataRecorder::get()->addDataTo("ToAutotrophFlux", toFlux_);
  toFlux_ = 0;
}

void Autotrophs::update() {
  double growthVolume = nutrient_.getVolume();
  addToVolume(growthVolume);
  nutrient_.subtractFromVolume(growthVolume);
}

double Autotrophs::getVolume() {
  return volume_;
}

void Autotrophs::setVolume(const double volume) {
  volume_ = volume;
}

void Autotrophs::addToVolume(const double volume) {
  volume_ += volume;
  toFlux_ += volume;
}

void Autotrophs::subtractFromVolume(const double volume) {
  volume_ -= volume;
}
