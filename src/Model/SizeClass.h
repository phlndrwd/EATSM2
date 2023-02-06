#ifndef SIZECLASS_H
#define SIZECLASS_H

#include <queue>
#include <vector>

#include "Heterotroph.h"
#include "RandomSimple.h"

class SizeClass {
 public:
  SizeClass() = delete;
  SizeClass(RandomSimple&, const unsigned);

  void update();

  Heterotroph& getRandomHeterotroph();
  Heterotroph& getHeterotroph(const unsigned);
  const Heterotroph& getHeterotroph(const unsigned) const;
  Heterotroph removeHeterotroph(const unsigned);
  const Heterotroph removeHeterotroph(const unsigned) const;

  void addHeterotroph(Heterotroph);

 private:
  //void metabolisation(Heterotroph&);


  RandomSimple& random_;
  const unsigned maxPopulation_;

  std::vector<Heterotroph> heterotrophs_;
  std::vector<Heterotroph>::iterator pointer_;
  std::queue<unsigned> alive_;
  std::queue<unsigned> dead_;

};

#endif // SIZECLASS_H
