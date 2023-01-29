#include "Autotrophs.h"
#include "DataRecorder.h"
#include "InitialState.h"
#include "Nutrient.h"
#include "Parameters.h"

#include <iostream>

Autotrophs::Autotrophs(Nutrient& nutrient) : mNutrient(nutrient) {
  if (InitialState::Get()->IsInitialised() == true)
    mVolume = InitialState::Get()->GetAutotrophVolume();
  else
    mVolume = Parameters::Get()->GetInitialAutotrophVolume();

  std::cout << "Autotroph pool created." << std::endl;
}

Autotrophs::~Autotrophs() {}

void Autotrophs::RecordData() {
  DataRecorder::Get()->AddDataTo("AutotrophVolume", mVolume);
  DataRecorder::Get()->AddDataTo("ToAutotrophFlux", mToFlux);
  mToFlux = 0;
}

void Autotrophs::Update() {
  double growthVolume = mNutrient.GetVolume();
  AddToVolume(growthVolume);
  mNutrient.SubtractFromVolume(growthVolume);
}

double Autotrophs::GetVolume() { return mVolume; }

void Autotrophs::SetVolume(const double volume) { mVolume = volume; }

void Autotrophs::AddToVolume(const double volume) {
  mVolume += volume;
  mToFlux += volume;
}

void Autotrophs::SubtractFromVolume(const double volume) { mVolume -= volume; }
