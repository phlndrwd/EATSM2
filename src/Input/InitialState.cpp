#include "InitialState.h"

#include "HeritableTraits.h"
#include "Heterotroph.h"
#include "HeterotrophProcessor.h"
#include "Parameters.h"
#include "Strings.h"

InitialState* InitialState::this_ = nullptr;

InitialState* InitialState::Get() {
  if (this_ == nullptr) this_ = new InitialState();

  return this_;
}

InitialState::~InitialState() {
  if (this_ != nullptr)
    delete this_;
}

InitialState::InitialState() {
  isInitialised_ = false;
}

bool InitialState::Initialise(const std::vector<std::vector<std::string>>& rawInitialStateData) {
  // Model variables
  nutrientVolume_ = Strings::stringToNumber(rawInitialStateData[constants::kStateLineNutrientVol][0]);
  autotrophVolume_ = Strings::stringToNumber(rawInitialStateData[constants::kStateLineAutotrophVol][0]);
  // Heterotrophs
  initialPopulationSize_ = 0;

  HeterotrophProcessor heterotrophProcessor;

  heterotrophs_.resize(Parameters::Get()->getNumberOfSizeClasses());
  for (unsigned lineIndex = constants::kStateLineFirstHeterotroph; lineIndex < rawInitialStateData.size();
       ++lineIndex) {
    double traitValue = Strings::stringToNumber(rawInitialStateData[lineIndex][0]);
    double volumeActual = Strings::stringToNumber(rawInitialStateData[lineIndex][1]);
    unsigned sizeClassIndex = Strings::stringToNumber(rawInitialStateData[lineIndex][2]);
    double volumeHeritable = heterotrophProcessor.traitValueToVolume(traitValue);
    std::vector<double> heritableTraitValues{traitValue};
    std::vector<bool> areTraitsMutant{false};
    HeritableTraits heritableTraits(heritableTraitValues, areTraitsMutant);
    Heterotroph* individual =
        new Heterotroph(heritableTraits, volumeHeritable, volumeActual, sizeClassIndex);
    heterotrophs_[sizeClassIndex].push_back(individual);
    ++initialPopulationSize_;
  }
  isInitialised_ = true;
  // Currently no circumstance under which this function can return false.
  return isInitialised_;
}

double& InitialState::getNutrientVolume() {
  return nutrientVolume_;
}

double& InitialState::getAutotrophVolume() {
  return autotrophVolume_;
}

std::vector<std::vector<Heterotroph*>>& InitialState::getHeterotrophs() {
  return heterotrophs_;
}

unsigned& InitialState::getInitialPopulationSize() {
  return initialPopulationSize_;
}

bool InitialState::isInitialised() {
  return isInitialised_;
}
