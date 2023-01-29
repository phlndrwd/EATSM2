#ifndef HERITABLETRAITS
#define HERITABLETRAITS

#include "Constants.h"
#include "RandomSimple.h"

class HeritableTraits {
 public:
  HeritableTraits(const Types::DoubleVector&, const Types::BoolVector&);
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

  Types::DoubleVector mValues;
  Types::BoolVector mAreMutantTraits;
};

#endif
