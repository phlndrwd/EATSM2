#ifndef ENCOUNTERALGORITHM
#define ENCOUNTERALGORITHM

#include "Enums.h"
#include "SizeClass.h"

#include <iterator>
#include <vector>

class EncounterAlgorithm {
 public:
  EncounterAlgorithm(const unsigned);

  void update(std::vector<SizeClass>&, SizeClass&, std::vector<structs::MovingHeterotroph>&);

 private:
  std::vector<SizeClass>::iterator calcFeedingProbability(std::vector<SizeClass>&, SizeClass&);
  void calcEffectiveSizeClassVolumes(std::vector<SizeClass>&, SizeClass&, std::vector<double>&);
  std::vector<size_t> getPopulationSizes(std::vector<SizeClass>&, SizeClass& sizeClass);
  std::vector<SizeClass>::iterator setCoupledSizeClass(const std::vector<double>&, std::vector<SizeClass>&);
  void calcFeedingStrategy();

  double functionalResponseLinear(const unsigned, const double);
  double functionalResponseNonLinear(const unsigned, const double);

  void feedFromAutotrophs(Heterotroph&);
  void feedFromHeterotrophs(Heterotroph&, std::vector<SizeClass>::iterator);

  RandomSimple random_;

  const unsigned numberOfSizeClasses_;
  const std::vector<double> linearFeedingDenominators_;
  const std::vector<double> halfSaturationConstants_;

  enums::eFeedingStrategy feedingStrategy_;

  double effectivePreyVolume_;
  double effectiveAutotrophVolume_;
  double feedingProbabilty_;
};
#endif
