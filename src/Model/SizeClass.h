#ifndef SIZECLASS_H
#define SIZECLASS_H

#include <functional>
#include <iterator>
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
  void update(std::vector<structs::MovingHeterotroph>&);

  void sizeClassSubset(std::function<void(unsigned)>);

  size_t getPopulationSize();
  unsigned getRandomHeterotrophIndex();
  Heterotroph& getRandomHeterotroph();
  Heterotroph& getHeterotroph(const unsigned);
  const Heterotroph& getHeterotroph(const unsigned) const;
  Heterotroph& removeHeterotroph(const unsigned);
  Autotrophs& getAutotrophs();

  void addHeterotroph(Heterotroph);

  const std::vector<double>& getSizeClassPreferences() const;
  const std::vector<double>& getSizeClassVolumes() const;
  unsigned getIndex() const;

 private:

  void metabolisation();
  void starvation();

  void starve(const unsigned);

  Nutrient& nutrient_;
  const unsigned index_;  // index_ is being deprecated.

  const std::vector<double> sizeClassPreferences_;
  const std::vector<double> sizeClassVolumes_;
  const double sizeClassMidPoint_;
  const double sizeClassSubsetFraction_;
  const unsigned maxPopulation_;

  Autotrophs autotrophs_;
  HeterotrophProcessor heterotrophProcessor_;
  RandomSimple random_;

  std::vector<Heterotroph> heterotrophs_;
  std::vector<unsigned> alive_;
  std::queue<unsigned> dead_;
};

#endif // SIZECLASS_H
