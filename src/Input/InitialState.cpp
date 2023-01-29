#include "InitialState.h"

#include "HeritableTraits.h"
#include "Heterotroph.h"
#include "HeterotrophProcessor.h"
#include "Parameters.h"
#include "Strings.h"

InitialState* InitialState::mThis = nullptr;

InitialState* InitialState::Get() {
  if (mThis == nullptr) mThis = new InitialState();

  return mThis;
}

InitialState::~InitialState() {
  if (mThis != nullptr)
    delete mThis;
}

InitialState::InitialState() { mIsInitialised = false; }

bool InitialState::Initialise(const std::vector<std::vector<std::string>>& rawInitialStateData) {
  // Model variables
  mNutrientVolume = Strings::StringToNumber(rawInitialStateData[Constants::cStateLineNutrientVol][0]);
  mAutotrophVolume = Strings::StringToNumber(rawInitialStateData[Constants::cStateLineAutotrophVol][0]);
  // Heterotrophs
  mInitialPopulationSize = 0;

  HeterotrophProcessor heterotrophProcessor;

  mHeterotrophs.resize(Parameters::Get()->GetNumberOfSizeClasses());
  for (unsigned lineIndex = Constants::cStateLineFirstHeterotroph; lineIndex < rawInitialStateData.size();
       ++lineIndex) {
    double traitValue = Strings::StringToNumber(rawInitialStateData[lineIndex][0]);
    double volumeActual = Strings::StringToNumber(rawInitialStateData[lineIndex][1]);
    unsigned sizeClassIndex = Strings::StringToNumber(rawInitialStateData[lineIndex][2]);
    double volumeHeritable = heterotrophProcessor.TraitValueToVolume(traitValue);
    std::vector<double> heritableTraitValues{traitValue};
    std::vector<bool> areTraitsMutant{false};
    HeritableTraits heritableTraits(heritableTraitValues, areTraitsMutant);
    Heterotroph* individual =
        new Heterotroph(heritableTraits, volumeHeritable, volumeActual, sizeClassIndex);
    mHeterotrophs[sizeClassIndex].push_back(individual);
    ++mInitialPopulationSize;
  }
  mIsInitialised = true;
  return mIsInitialised;  // Currently no circumstance under which this function
                          // can return false.
}

double& InitialState::GetNutrientVolume() { return mNutrientVolume; }

double& InitialState::GetAutotrophVolume() { return mAutotrophVolume; }

std::vector<std::vector<Heterotroph*>>& InitialState::GetHeterotrophs() { return mHeterotrophs; }

unsigned& InitialState::GetInitialPopulationSize() { return mInitialPopulationSize; }

bool InitialState::IsInitialised() { return mIsInitialised; }
