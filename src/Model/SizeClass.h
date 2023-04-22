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
  explicit SizeClass(std::vector<SizeClass>&, Nutrient&, const double, const double, const unsigned, const unsigned);

  void populate(const double);
  std::vector<structs::MovingHeterotroph> update();

  size_t getPopulationSize();
  unsigned getRandomHeterotrophIndex();
  Heterotroph& getRandomHeterotroph();
  Heterotroph& getHeterotroph(const unsigned);
  const Heterotroph& getHeterotroph(const unsigned) const;
  Heterotroph& removeHeterotroph(const unsigned);

  void addHeterotroph(Heterotroph);

 private:
  void feeding();
  void metabolisation();
  void starvation();

  void sizeClassSubset(std::function<void(unsigned)>);

  std::vector<size_t> getPopulationSizes();
  void calcFeedingProbability(std::vector<size_t>&);
  void calcEffectiveSizeClassVolumes(std::vector<size_t>&, std::vector<double>&);
  void setCoupledSizeClass(std::vector<double>&);
  void calcFeedingStrategy();

  void feedFromAutotrophs(Heterotroph&);
  void feedFromHeterotrophs(Heterotroph&);

  void starve(const unsigned);

  std::vector<SizeClass>& sizeClasses_;
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
  std::vector<SizeClass>::iterator autotrophSizeClassIt_;
  std::vector<SizeClass>::iterator coupledSizeClassIt_;
  enums::eFeedingStrategy feedingStrategy_;
};

#endif // SIZECLASS_H
