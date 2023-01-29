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

  HeritableTraits getChildTraits(RandomSimple&);
  const std::vector<double>& getValues() const;
  const std::vector<bool>& areTraitsMutant() const;

  const double& getValue(const constants::eHeritableTraitIndices) const;
  bool isTraitMutant(const unsigned) const;

  void setValue(const constants::eHeritableTraitIndices, const double);

 private:
  const double mutationProbability_;
  const double mutationStandardDeviation_;

  std::vector<double> values_;
  std::vector<bool> areMutantTraits_;
};

#endif
