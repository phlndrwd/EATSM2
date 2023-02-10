#ifndef SIZECLASS_H
#define SIZECLASS_H

#include <queue>
#include <vector>

#include "Heterotroph.h"
#include "HeterotrophProcessor.h"
#include "RandomSimple.h"

class SizeClass {
 public:
  SizeClass() = delete;
  SizeClass(RandomSimple&, const double, const unsigned);

  void update();

  Heterotroph& getRandomHeterotroph();
  Heterotroph& getHeterotroph(const unsigned);
  const Heterotroph& getHeterotroph(const unsigned) const;
  Heterotroph removeHeterotroph(const unsigned);
  const Heterotroph removeHeterotroph(const unsigned) const;

  void addHeterotroph(Heterotroph);

 private:
  HeterotrophProcessor heterotrophProcessor_;

  RandomSimple& random_;
  const double sizeClassMidPoint_;
  const unsigned maxPopulation_;

  std::vector<Heterotroph> heterotrophs_;
  std::vector<Heterotroph>::iterator pointer_;
  std::queue<unsigned> alive_;
  std::queue<unsigned> dead_;

};

#endif // SIZECLASS_H
