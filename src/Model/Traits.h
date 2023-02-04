#ifndef TRAITS
#define TRAITS

#include <vector>

#include "Constants.h"

class RandomSimple;

class Traits {
 public:
  Traits() = delete;
  Traits(const std::vector<double>&, const std::vector<bool>&);
  ~Traits();

  Traits(const Traits&);
  Traits(Traits&&) noexcept;

  Traits& operator=(const Traits&);
  Traits& operator=(Traits&&) noexcept;

  const Traits getChildTraits(RandomSimple&);
  const std::vector<double>& getValues() const;
  const std::vector<bool>& areTraitsMutant() const;

  const double& getValue(const constants::eHeritableTraitIndices) const;
  bool isTraitMutant(const unsigned) const;

  void setValue(const constants::eHeritableTraitIndices, const double);

 private:
  double mutationProbability_;
  double mutationStandardDeviation_;

  std::vector<double> values_;
  std::vector<bool> areMutantTraits_;
};

#endif
