#include "Heterotrophs.h"

#include <algorithm>
#include <iostream>

#include "Autotrophs.h"
#include "HeritableTraits.h"
#include "Heterotroph.h"
#include "InitialState.h"
#include "Nutrient.h"
#include "Parameters.h"
#include "RandomSimple.h"

Heterotrophs::Heterotrophs(Nutrient& nutrient, Autotrophs& autotrophs)
    : mNutrient(nutrient),
      mAutotrophs(autotrophs),
      mHeterotrophProcessor(),
      mHeterotrophData(),
      mTimer(),
      mInterSizeClassPreferenceMatrix(Parameters::Get()->GetInterSizeClassPreferenceMatrix()),
      mInterSizeClassVolumeMatrix(Parameters::Get()->GetInterSizeClassVolumeMatrix()),
      mSizeClassMidPoints(Parameters::Get()->GetSizeClassMidPoints()),
      mMaximumSizeClassPopulations(Parameters::Get()->GetMaximumSizeClassPopulations()),
      mSmallestIndividualVolume(Parameters::Get()->GetSmallestIndividualVolume()),
      mInitialHeterotrophicVolume(Parameters::Get()->GetInitialHeterotrophVolume()),
      mSizeClassSubsetFraction(Parameters::Get()->GetSizeClassSubsetFraction()),
      mNumberOfSizeClasses(Parameters::Get()->GetNumberOfSizeClasses()),
      mPreferredPreyVolumeRatio(Parameters::Get()->GetPreferredPreyVolumeRatio()),
      mAutotrophSizeClassIndex(Parameters::Get()->GetAutotrophSizeClassIndex()) {
  CreateInitialPopulation();
}

Heterotrophs::~Heterotrophs() {
  for (unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex) {
    for (std::size_t individualIndex = 0; individualIndex < GetSizeClassPopulation(sizeClassIndex); ++individualIndex)
      delete mIndividuals[sizeClassIndex][individualIndex];

    mIndividuals[sizeClassIndex].clear();
  }
  mIndividuals.clear();
}

void Heterotrophs::CreateInitialPopulation() {
  mIndividuals.resize(mNumberOfSizeClasses);
  mIndividualsDead.resize(mNumberOfSizeClasses);
  mFedCount.resize(mNumberOfSizeClasses);

  mTestStructure = new TestStruct[mNumberOfSizeClasses];

  for (unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex) {
    mIndividuals[sizeClassIndex].reserve(mMaximumSizeClassPopulations[sizeClassIndex]);
    mTestStructure[sizeClassIndex].mAge = 0;
  }

  if (InitialState::Get()->IsInitialised() == false) {
    unsigned initialPopulationSize = 0;
    double secondaryProducerVolume = mSmallestIndividualVolume * mPreferredPreyVolumeRatio;

    unsigned firstPopulatedIndex = mHeterotrophProcessor.FindSizeClassIndexFromVolume(secondaryProducerVolume);
    double individualVolume = mSizeClassMidPoints[firstPopulatedIndex];
    double traitValue = mHeterotrophProcessor.VolumeToTraitValue(individualVolume);

    double initialHeterotrophVolume = mInitialHeterotrophicVolume;

    while (individualVolume <= initialHeterotrophVolume) {
      initialHeterotrophVolume -= individualVolume;
      std::vector<double> heritableTraitValues{traitValue};
      std::vector<bool> areTraitsMutant{false};
      HeritableTraits heritableTraits(heritableTraitValues, areTraitsMutant);
      mIndividuals[firstPopulatedIndex].push_back(
          new Heterotroph(heritableTraits, individualVolume, firstPopulatedIndex));
      ++initialPopulationSize;
    }

    if (initialHeterotrophVolume > 0) mNutrient.AddToVolume(initialHeterotrophVolume);

    std::cout << "A single heterotrophic size class initialised with " << initialPopulationSize + 1 << " individuals."
              << std::endl;
  } else {
    mIndividuals = InitialState::Get()->GetHeterotrophs();
    std::cout << "Heterotrophic size classes initialised with " << InitialState::Get()->GetInitialPopulationSize()
              << " individuals." << std::endl;
  }
}

void Heterotrophs::Update() {
  // Feeding
  mTimer.Start();
  Feeding();
  mHeterotrophData.AddToTimeFeeding(mTimer.Stop());
  // Metabolising
  mTimer.Start();
  Metabolisation();
  mHeterotrophData.AddToTimeMetabolising(mTimer.Stop());
  // Starving
  mTimer.Start();
  Starvation();
  mHeterotrophData.AddToTimeStarving(mTimer.Stop());
  // Reproducing
  mTimer.Start();
  Reproduction();
  mHeterotrophData.AddToTimeReproducing(mTimer.Stop());
}

bool Heterotrophs::RecordData() {
  mHeterotrophData.InitialiseDataStructures();
  for (unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex) {
    std::size_t sizeClassPopulation = GetSizeClassPopulation(sizeClassIndex);
    for (std::size_t individualIndex = 0; individualIndex < sizeClassPopulation; ++individualIndex) {
      mHeterotrophData.AddIndividualData(mIndividuals[sizeClassIndex][individualIndex]);
    }
    double sizeClassMultiplier = 1 / (double)sizeClassPopulation;
    mHeterotrophData.AddSizeClassData(sizeClassIndex, sizeClassPopulation, sizeClassMultiplier);
  }
  mHeterotrophData.NormaliseData();
  mHeterotrophData.RecordOutputData();

  return mHeterotrophData.AreHeterotrophsAlive();
}

std::size_t Heterotrophs::GetSizeClassPopulation(const unsigned sizeClassIndex) const {
  return mIndividuals[sizeClassIndex].size();
}

Types::HeterotrophPointer Heterotrophs::GetIndividual(const unsigned sizeClassIndex, const unsigned individualIndex) {
  return mIndividuals[sizeClassIndex][individualIndex];
}

void Heterotrophs::CalculateFeedingProbabilities() {
  for (unsigned predatorIndex = 0; predatorIndex < mNumberOfSizeClasses; ++predatorIndex) {
    mFedCount[predatorIndex] = 0;
    if (GetSizeClassPopulation(predatorIndex) > 0) {
      double effectivePreyVolume = 0;
      double highestEffectiveSizeClassVolume = 0;
      unsigned coupledSizeClassIndex = 0;

      for (unsigned preyIndex = 0; preyIndex < mNumberOfSizeClasses; ++preyIndex) {
        double effectiveSizeClassVolume = 0;
        // Add the result of the autotroph volume - no frequency coefficient.
        if (preyIndex == mAutotrophSizeClassIndex)
          effectiveSizeClassVolume =
              mInterSizeClassPreferenceMatrix[predatorIndex][preyIndex] * mAutotrophs.GetVolume();
        // Add the result of the heterotrophs.
        std::size_t preySizeClassPopulation = GetSizeClassPopulation(preyIndex);
        if (preySizeClassPopulation > 0) {
          if (preyIndex != predatorIndex)
            effectiveSizeClassVolume += mInterSizeClassVolumeMatrix[predatorIndex][preyIndex] * preySizeClassPopulation;
          else
            effectiveSizeClassVolume +=
                mInterSizeClassVolumeMatrix[predatorIndex][preyIndex] * (preySizeClassPopulation - 1);
        }
        if (effectiveSizeClassVolume > highestEffectiveSizeClassVolume) {
          highestEffectiveSizeClassVolume = effectiveSizeClassVolume;
          coupledSizeClassIndex = preyIndex;
        }
        mHeterotrophData.SetEffectiveSizeClassVolume(predatorIndex, preyIndex, effectiveSizeClassVolume);
        effectivePreyVolume += effectiveSizeClassVolume;
      }
      mHeterotrophData.SetEffectivePreyVolume(predatorIndex, effectivePreyVolume);
      mHeterotrophData.SetFeedingProbability(
          predatorIndex, mHeterotrophProcessor.CalculateFeedingProbability(predatorIndex, effectivePreyVolume));
      mHeterotrophData.SetCoupledSizeClassIndex(predatorIndex, coupledSizeClassIndex);
    }
  }
}

void Heterotrophs::Feeding() {
  CalculateFeedingProbabilities();

  for (unsigned predatorIndex = 0; predatorIndex < mNumberOfSizeClasses; ++predatorIndex) {
    std::size_t sizeClassPopulation = GetSizeClassPopulation(predatorIndex);

    if (sizeClassPopulation > 0) {
      unsigned sizeClassPopulationSubset =
          mHeterotrophProcessor.RoundWithProbability(mRandom, sizeClassPopulation * mSizeClassSubsetFraction);

      for (unsigned potentialEncounterIndex = 0; potentialEncounterIndex < sizeClassPopulationSubset;
           ++potentialEncounterIndex) {
        if (mRandom.GetUniform() <= mHeterotrophData.GetFeedingProbability(predatorIndex)) {
          Types::HeterotrophPointer predator = GetRandomPredatorFromSizeClass(predatorIndex);
          if (predator != nullptr) {
            unsigned coupledIndex = mHeterotrophData.GetCoupledSizeClassIndex(predatorIndex);
            if (coupledIndex == mAutotrophSizeClassIndex)
              FeedFromAutotrophs(predator);
            else
              FeedFromHeterotrophs(predator, coupledIndex);
          }
        }
      }
    }
  }
  DeleteDead();
}

void Heterotrophs::Metabolisation() {
  for (unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex) {
    for (std::size_t individualIndex = 0; individualIndex < GetSizeClassPopulation(sizeClassIndex); ++individualIndex) {
      Types::HeterotrophPointer individual = GetIndividual(sizeClassIndex, individualIndex);

      double metabolicDeduction = mHeterotrophProcessor.CalculateMetabolicDeduction(individual);

      if ((individual->GetVolumeActual() - metabolicDeduction) > 0) {
        individual->SetHasFed(false);  // Reset for the next time step
        double waste = individual->Metabolise(metabolicDeduction);
        mNutrient.AddToVolume(waste);

        // Individuals can move up a size class from having consumed a
        // lot. They need to move after this function has completed to
        // avoid handling them twice.
        if (mHeterotrophProcessor.UpdateSizeClassIndex(individual) == true) StageForMoving(individual, sizeClassIndex);

      } else
        Starve(individual);
    }
  }
  MoveIndividuals();
  DeleteDead();
}

void Heterotrophs::Starvation() {
  for (unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex) {
    std::size_t sizeClassPopulation = GetSizeClassPopulation(sizeClassIndex);

    if (sizeClassPopulation > 0) {
      unsigned sizeClassSubsetSize =
          mHeterotrophProcessor.RoundWithProbability(mRandom, sizeClassPopulation * mSizeClassSubsetFraction);

      for (unsigned potentialStarvation = 0; potentialStarvation < sizeClassSubsetSize; ++potentialStarvation) {
        Types::HeterotrophPointer individual = GetRandomIndividualFromSizeClass(sizeClassIndex);

        if (individual != nullptr)
          if (mRandom.GetUniform() <= mHeterotrophProcessor.CalculateStarvationProbability(individual))
            Starve(individual);
      }
    }
  }
  DeleteDead();
}

void Heterotrophs::Reproduction() {
  for (unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex) {
    for (std::size_t individualIndex = 0; individualIndex < GetSizeClassPopulation(sizeClassIndex); ++individualIndex) {
      Types::HeterotrophPointer individual = GetIndividual(sizeClassIndex, individualIndex);
      if (individual->GetVolumeActual() >= individual->GetVolumeReproduction()) {
        Types::HeterotrophPointer childIndividual = individual->Reproduce(mRandom, mHeterotrophProcessor);

        // Parent data needs to be collected before size class is updated.
        mHeterotrophData.IncrementParentFrequencies(individual->GetSizeClassIndex());
        if (mHeterotrophProcessor.UpdateSizeClassIndex(individual) == true) StageForMoving(individual, sizeClassIndex);

        if (childIndividual->GetHeritableTraits().IsTraitMutant(Constants::eVolume) == true) {
          mHeterotrophProcessor.UpdateSizeClassIndex(
              childIndividual);  // Mutants may inherit more than a fixed fraction of the parent's volume
          mHeterotrophData.IncrementMutantFrequency(childIndividual->GetSizeClassIndex(), Constants::eVolume);
        }

        mHeterotrophData.IncrementChildFrequencies(childIndividual->GetSizeClassIndex());
        mChildren.push_back(childIndividual);
      }
    }
  }
  MoveIndividuals();
  AddChildren();
}

void Heterotrophs::FeedFromAutotrophs(Types::HeterotrophPointer grazer) {
  if (mAutotrophs.GetVolume() > mSmallestIndividualVolume) {
    mAutotrophs.SubtractFromVolume(mSmallestIndividualVolume);
    mHeterotrophData.IncrementVegetarianFrequencies(grazer);

    double waste = grazer->ConsumePreyVolume(mSmallestIndividualVolume);
    ++mFedCount[grazer->GetSizeClassIndex()];

    mHeterotrophProcessor.UpdateHerbivoreTrophicIndex(grazer);
    mNutrient.AddToVolume(waste);
  }
}

void Heterotrophs::FeedFromHeterotrophs(Types::HeterotrophPointer predator, const unsigned coupledIndex) {
  Types::HeterotrophPointer prey = GetRandomPreyFromSizeClass(coupledIndex, predator);
  if (prey != nullptr) {
    double preyVolume = prey->GetVolumeActual();
    mHeterotrophData.IncrementCarnivoreFrequencies(predator, prey);

    double waste = predator->ConsumePreyVolume(preyVolume);
    ++mFedCount[predator->GetSizeClassIndex()];

    mHeterotrophProcessor.UpdateCarnivoreTrophicIndex(predator, prey);
    mNutrient.AddToVolume(waste);
    Kill(prey);
  }
}

// Used for starvation. Hence check against number of dead
Types::HeterotrophPointer Heterotrophs::GetRandomIndividualFromSizeClass(const unsigned sizeClassIndex) {
  std::size_t numberLiving = GetSizeClassPopulation(sizeClassIndex);
  unsigned numberActive = numberLiving - mIndividualsDead[sizeClassIndex].size();

  Types::HeterotrophPointer randomIndividual = nullptr;
  if (numberActive > 0) {
    unsigned count = 0;
    do {
      ++count;
      unsigned randomIndividualIndex = mRandom.GetUniformInt(numberLiving - 1);
      randomIndividual = GetIndividual(sizeClassIndex, randomIndividualIndex);
      if (count > numberActive) break;
    } while (randomIndividual->IsDead() == true);
  }
  return randomIndividual;
}

// Potential predators should feed only once in a discrete time step. Not likely, but they may have already been marked
// as dead.
Types::HeterotrophPointer Heterotrophs::GetRandomPredatorFromSizeClass(const unsigned sizeClassIndex) {
  std::size_t numberLiving = GetSizeClassPopulation(sizeClassIndex);
  unsigned numberActive = numberLiving - mIndividualsDead[sizeClassIndex].size() - mFedCount[sizeClassIndex];

  Types::HeterotrophPointer randomIndividual = nullptr;
  if (numberActive > 0) {
    unsigned count = 0;
    do {
      ++count;
      unsigned randomIndividualIndex = mRandom.GetUniformInt(numberLiving - 1);
      randomIndividual = GetIndividual(sizeClassIndex, randomIndividualIndex);
      if (count > numberActive) break;
    } while (randomIndividual->IsDead() == true || randomIndividual->HasFed() == true);
  }
  return randomIndividual;
}

// Potential prey may already be marked as dead. They cannot be eaten by themselves.
Types::HeterotrophPointer Heterotrophs::GetRandomPreyFromSizeClass(const unsigned sizeClassIndex,
                                                                   const Types::HeterotrophPointer predator) {
  std::size_t numberLiving = GetSizeClassPopulation(sizeClassIndex);
  unsigned numberActive = numberLiving - mIndividualsDead[sizeClassIndex].size();

  // Only applicable when predator feeds from its own size class. Check
  // ensures numberActive does not go out of bounds
  if (predator->GetSizeClassIndex() == sizeClassIndex && numberActive > 0) --numberActive;

  Types::HeterotrophPointer randomIndividual = nullptr;
  if (numberActive > 0) {
    unsigned count = 0;
    do {
      ++count;
      unsigned randomIndividualIndex = mRandom.GetUniformInt(numberLiving - 1);
      randomIndividual = GetIndividual(sizeClassIndex, randomIndividualIndex);
      if (count > numberActive) break;
    } while (randomIndividual->IsDead() == true || randomIndividual == predator);
  }
  return randomIndividual;
}

void Heterotrophs::StageForMoving(Types::HeterotrophPointer individual, const unsigned currentSizeClassIndex) {
  mIndividualsToMove.push_back(std::make_pair(individual, currentSizeClassIndex));
}

void Heterotrophs::MoveSizeClass(Types::HeterotrophPointer individual, const unsigned currentSizeClassIndex) {
  if (RemoveFromSizeClass(individual, currentSizeClassIndex) == true)
    mIndividuals[individual->GetSizeClassIndex()].push_back(individual);
}

bool Heterotrophs::RemoveFromSizeClass(Types::HeterotrophPointer individual, const unsigned sizeClassIndex) {
  std::vector<Types::HeterotrophPointer>::iterator toErase;
  toErase = std::find(mIndividuals[sizeClassIndex].begin(), mIndividuals[sizeClassIndex].end(), individual);

  // And then erase if found
  if (toErase != mIndividuals[sizeClassIndex].end()) {
    // Delete when necessary in calling function
    mIndividuals[sizeClassIndex].erase(toErase);
    return true;
  }
  return false;
}

void Heterotrophs::DeleteIndividual(Types::HeterotrophPointer individual) {
  RemoveFromSizeClass(individual, individual->GetSizeClassIndex());
  delete individual;
}

void Heterotrophs::Starve(Types::HeterotrophPointer individual) {
  mNutrient.AddToVolume(individual->GetVolumeActual());
  mHeterotrophData.IncrementStarvedFrequencies(individual->GetSizeClassIndex());
  Kill(individual);
}

void Heterotrophs::Kill(Types::HeterotrophPointer individual) {
  individual->Kill();
  mIndividualsDead[individual->GetSizeClassIndex()].push_back(individual);
}

void Heterotrophs::AddChildren() {
  for (std::size_t childIndex = 0; childIndex < mChildren.size(); ++childIndex) {
    Types::HeterotrophPointer child = mChildren[childIndex];
    mIndividuals[child->GetSizeClassIndex()].push_back(child);
  }
  mChildren.clear();
}

void Heterotrophs::MoveIndividuals() {
  for (unsigned index = 0; index < mIndividualsToMove.size(); ++index) {
    MoveSizeClass(mIndividualsToMove[index].first, mIndividualsToMove[index].second);
  }
  mIndividualsToMove.clear();
}

void Heterotrophs::DeleteDead() {
  for (unsigned sizeClassIndex = 0; sizeClassIndex < mNumberOfSizeClasses; ++sizeClassIndex) {
    for (unsigned individualIndex = 0; individualIndex < mIndividualsDead[sizeClassIndex].size(); ++individualIndex)
      DeleteIndividual(mIndividualsDead[sizeClassIndex][individualIndex]);

    mIndividualsDead[sizeClassIndex].clear();
  }
}
