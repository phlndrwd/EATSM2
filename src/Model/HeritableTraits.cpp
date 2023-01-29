#include "HeritableTraits.h"

#include "Parameters.h"
#include "RandomSimple.h"

HeritableTraits::HeritableTraits(const std::vector<double>& values, const std::vector<bool>& areTraitsMutations)
    : mutationProbability_(Parameters::Get()->getMutationProbability()),
      mutationStandardDeviation_(Parameters::Get()->getMutationStandardDeviation()) {
  for (unsigned i = 0; i < values.size(); ++i) {
    values_.push_back(values[i]);
    areMutantTraits_.push_back(areTraitsMutations[i]);
  }
}

HeritableTraits::~HeritableTraits() { values_.clear(); }

HeritableTraits& HeritableTraits::operator=(const HeritableTraits& heritableTraits) {
  if (this != &heritableTraits) {
    values_ = heritableTraits.values_;
    areMutantTraits_ = heritableTraits.areMutantTraits_;
  }
  return *this;
}

HeritableTraits HeritableTraits::getChildTraits(RandomSimple& random) {
  std::size_t numberOfGenes = values_.size();
  std::vector<double> childValues = values_;
  std::vector<bool> areTraitsMutations(numberOfGenes, false);

  if (mutationProbability_ > 0) {
    for (std::size_t i = 0; i < numberOfGenes; ++i) {
      if (random.getUniform() <= mutationProbability_) {
        areTraitsMutations[i] = true;

        double mutationValue = random.getNormal(0.0, mutationStandardDeviation_);

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

const std::vector<bool>& HeritableTraits::areTraitsMutant() const {
  return areMutantTraits_;
}

const std::vector<double>& HeritableTraits::getValues() const {
  return values_;
}

bool HeritableTraits::isTraitMutant(const unsigned traitIndex) const {
  return areMutantTraits_[traitIndex];
}

const double& HeritableTraits::getValue(const constants::eHeritableTraitIndices trait) const {
  return values_[trait];
}

void HeritableTraits::setValue(const constants::eHeritableTraitIndices trait, const double traitValue) {
  values_[trait] = traitValue;
}
