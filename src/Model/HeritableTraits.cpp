#include "HeritableTraits.h"

#include "Parameters.h"
#include "RandomSimple.h"

HeritableTraits::HeritableTraits(const Types::DoubleVector& values, const Types::BoolVector& areTraitsMutations)
    : mMutationProbability(Parameters::Get()->GetMutationProbability()),
      mMutationStandardDeviation(Parameters::Get()->GetMutationStandardDeviation()) {
  for (unsigned i = 0; i < values.size(); ++i) {
    mValues.push_back(values[i]);
    mAreMutantTraits.push_back(areTraitsMutations[i]);
  }
}

HeritableTraits::~HeritableTraits() { mValues.clear(); }

HeritableTraits& HeritableTraits::operator=(const HeritableTraits& heritableTraits) {
  if (this != &heritableTraits) {
    mValues = heritableTraits.mValues;
    mAreMutantTraits = heritableTraits.mAreMutantTraits;
  }
  return *this;
}

HeritableTraits HeritableTraits::GetChildTraits(RandomSimple& random) {
  std::size_t numberOfGenes = mValues.size();
  std::vector<double> childValues = mValues;
  std::vector<bool> areTraitsMutations(numberOfGenes, false);

  if (mMutationProbability > 0) {
    for (std::size_t i = 0; i < numberOfGenes; ++i) {
      if (random.GetUniform() <= mMutationProbability) {
        areTraitsMutations[i] = true;

        double mutationValue = random.GetNormal(0.0, mMutationStandardDeviation);

        childValues[i] += mutationValue;

        // Perform reflection on mutations outside the required range 0 to 1.
        if (childValues[i] < 0) {
          childValues[i] = 0 - childValues[i];
        } else if (childValues[i] > 1) {
          childValues[i] = 2 - childValues[i];
        }
      }
    }
  }
  return HeritableTraits(childValues, areTraitsMutations);
}

const std::vector<bool>& HeritableTraits::AreTraitsMutant() const {
  return mAreMutantTraits;
}

const std::vector<double>& HeritableTraits::GetValues() const {
  return mValues;
}

bool HeritableTraits::IsTraitMutant(const unsigned traitIndex) const {
  return mAreMutantTraits[traitIndex];
}

const double& HeritableTraits::GetValue(const Constants::eHeritableTraitIndices trait) const {
  return mValues[trait];
}

void HeritableTraits::SetValue(const Constants::eHeritableTraitIndices trait, const double traitValue) {
  mValues[trait] = traitValue;
}
