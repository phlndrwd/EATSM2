#ifndef SIZECLASS_H
#define SIZECLASS_H

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

  unsigned getRandomHeterotrophIndex();
  Heterotroph& getRandomHeterotroph();
  Heterotroph& getHeterotroph(const unsigned);
  const Heterotroph& getHeterotroph(const unsigned) const;
  Heterotroph removeHeterotroph(const unsigned);
  const Heterotroph removeHeterotroph(const unsigned) const;

  void addHeterotroph(Heterotroph);

 private:
  void metabolisation(Nutrient&);
  void starvation(Nutrient&);

  void starve(Nutrient&, const unsigned);

  HeterotrophProcessor heterotrophProcessor_;

  HeterotrophData& heterotrophData_;
  RandomSimple random_;
  const double sizeClassMidPoint_;
  const double sizeClassSubsetFraction_;

  std::vector<Heterotroph> heterotrophs_;
  //std::vector<Heterotroph>::iterator pointer_;
  std::vector<unsigned> alive_;
  std::queue<unsigned> dead_;

};

#endif // SIZECLASS_H
