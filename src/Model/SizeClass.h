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
  SizeClass(HeterotrophData&, const double, const double, const unsigned, const unsigned);

  std::vector<Heterotroph> update(Nutrient&);

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

  HeterotrophProcessor heterotrophProcessor_;

  HeterotrophData& heterotrophData_;
  const double sizeClassMidPoint_;
  const double sizeClassSubsetFraction_;
  const unsigned maxPopulation_;
  RandomSimple random_;

  std::vector<Heterotroph> heterotrophs_;
  //std::vector<Heterotroph>::iterator pointer_;
  std::vector<unsigned> alive_;
  std::queue<unsigned> dead_;

  std::vector<double> preferenceVector;

};

#endif // SIZECLASS_H
