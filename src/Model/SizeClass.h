#ifndef SIZECLASS_H
#define SIZECLASS_H

#include <functional>
#include <queue>
#include <vector>

#include "Heterotroph.h"
#include "HeterotrophData.h"
#include "HeterotrophProcessor.h"
#include "Nutrient.h"
#include "RandomSimple.h"

class SizeClass {
 public:
  SizeClass() = delete;
  SizeClass(HeterotrophData&, const std::vector<double>, const std::vector<double>,
      const double, const double, const unsigned, const unsigned, const unsigned);

  std::vector<Heterotroph> update(Nutrient&);
  void calculateFeedingProbability(std::vector<size_t>& populationSizes);

  size_t getPopulationSize();
  unsigned getRandomHeterotrophIndex();
  Heterotroph& getRandomHeterotroph();
  Heterotroph& getHeterotroph(const unsigned);
  const Heterotroph& getHeterotroph(const unsigned) const;
  Heterotroph& removeHeterotroph(const unsigned);

  void addHeterotroph(Heterotroph);

 private:
  void feeding();
  void metabolisation(Nutrient&);
  void starvation(Nutrient&);

  void sizeClassSubset(std::function<void(unsigned)>);
  void starve(Nutrient&, const unsigned);

  HeterotrophData& heterotrophData_;
  const std::vector<double> sizeClassPreferences_;
  const std::vector<double> sizeClassVolumes_;
  const double sizeClassMidPoint_;
  const double sizeClassSubsetFraction_;

  const unsigned index_;
  const unsigned maxPopulation_;
  const unsigned autotrophSizeClassIndex_;

  RandomSimple random_;
  HeterotrophProcessor heterotrophProcessor_;

  std::vector<Heterotroph> heterotrophs_;
  //std::vector<Heterotroph>::iterator pointer_;
  std::vector<unsigned> alive_;
  std::queue<unsigned> dead_;
};

#endif // SIZECLASS_H
