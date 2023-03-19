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
  SizeClass(Nutrient&, Autotrophs&, const double, const unsigned, const unsigned);

  void populate(const double);
  std::vector<structs::MovingHeterotroph> update();
  void calculateFeedingProbability(std::vector<size_t>&);

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
  void starve(const unsigned);

  Nutrient& nutrient_;
  Autotrophs& autotrophs_;
  const unsigned index_;

  const std::vector<double> sizeClassPreferences_;
  const std::vector<double> sizeClassVolumes_;
  const double sizeClassMidPoint_;
  const double sizeClassSubsetFraction_;
  const unsigned maxPopulation_;
  const unsigned autotrophSizeClassIndex_;

  RandomSimple random_;
  HeterotrophProcessor heterotrophProcessor_;

  std::vector<Heterotroph> heterotrophs_;
  std::vector<unsigned> alive_;
  std::queue<unsigned> dead_;

  double effectivePreyVolume_;
  double feedingProbabilty_;
  unsigned coupledSizeClassIndex_;
  enums::eFeedingStrategy feedingStrategy_;
};

#endif // SIZECLASS_H
