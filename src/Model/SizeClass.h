#ifndef SIZECLASS_H
#define SIZECLASS_H

#include <functional>
#include <queue>
#include <vector>

#include "Autotrophs.h"
#include "Heterotroph.h"
#include "HeterotrophProcessor.h"
#include "Nutrient.h"
#include "RandomSimple.h"
#include "Structs.h"

class SizeClass {
 public:
  SizeClass() = delete;
  explicit SizeClass(Nutrient&, const double, const double, const unsigned, const unsigned);

  void populate(const double);
  std::vector<structs::MovingHeterotroph> update(std::vector<SizeClass>&);

  size_t getPopulationSize();
  unsigned getRandomHeterotrophIndex();
  Heterotroph& getRandomHeterotroph();
  Heterotroph& getHeterotroph(const unsigned);
  const Heterotroph& getHeterotroph(const unsigned) const;
  Heterotroph& removeHeterotroph(const unsigned);
  Autotrophs& getAutotrophs();

  void addHeterotroph(Heterotroph);

 private:
  void feeding(std::vector<SizeClass>& sizeClasses);
  void metabolisation();
  void starvation();

  void sizeClassSubset(std::function<void(unsigned)>);

  std::vector<size_t> getPopulationSizes(std::vector<SizeClass>&);
  std::vector<SizeClass>::iterator calcFeedingProbability(std::vector<SizeClass>&);
  void calcEffectiveSizeClassVolumes(std::vector<SizeClass>&, std::vector<double>&);
  std::vector<SizeClass>::iterator setCoupledSizeClass(const std::vector<double>&,
                                                       std::vector<SizeClass>& sizeClasses);
  void calcFeedingStrategy();

  void feedFromAutotrophs(Heterotroph&);
  void feedFromHeterotrophs(Heterotroph&, std::vector<SizeClass>::iterator&);

  void starve(const unsigned);

  Nutrient& nutrient_;
  const unsigned index_;  // index_ is being deprecated.

  const std::vector<double> sizeClassPreferences_;
  const std::vector<double> sizeClassVolumes_;
  const double sizeClassMidPoint_;
  const double sizeClassSubsetFraction_;
  const unsigned numberOfSizeClasses_;
  const unsigned maxPopulation_;

  Autotrophs autotrophs_;
  HeterotrophProcessor heterotrophProcessor_;
  RandomSimple random_;

  std::vector<Heterotroph> heterotrophs_;
  std::vector<unsigned> alive_;
  std::queue<unsigned> dead_;

  double effectivePreyVolume_;
  double effectiveAutotrophVolume_;
  double feedingProbabilty_;

  enums::eFeedingStrategy feedingStrategy_;
};

#endif // SIZECLASS_H
