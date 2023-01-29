#ifndef HERITABLETRAITS
#define HERITABLETRAITS

#include <vector>

#include "Constants.h"

class RandomSimple;

class HeritableTraits {
 public:
  HeritableTraits(const std::vector<double>&, const std::vector<bool>&);
  ~HeritableTraits();
  HeritableTraits& operator=(const HeritableTraits&);

  HeritableTraits GetChildTraits(RandomSimple&);
  const std::vector<double>& GetValues() const;
  const std::vector<bool>& AreTraitsMutant() const;

  const double& GetValue(const Constants::eHeritableTraitIndices) const;
  bool IsTraitMutant(const unsigned) const;

  void SetValue(const Constants::eHeritableTraitIndices, const double);

 private:
  const double mMutationProbability;
  const double mMutationStandardDeviation;

  std::vector<double> mValues;
  std::vector<bool> mAreMutantTraits;
};

#endif
