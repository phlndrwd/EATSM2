#ifndef HETEROTROPH
#define HETEROTROPH

#include "Traits.h"

class HeterotrophProcessor;
class RandomSimple;

class Heterotroph {
 public:
  Heterotroph() = delete;
  Heterotroph(const Traits&, const double, const unsigned);
  Heterotroph(const Traits&, const double, const double, const double, const double, const unsigned);
  Heterotroph(const Traits&, const double, const double, const unsigned);
  Heterotroph(const Heterotroph&);
  Heterotroph(const Heterotroph&&) noexcept;
  ~Heterotroph();
  Heterotroph& operator=(const Heterotroph&);
  Heterotroph& operator=(const Heterotroph&&);
  Heterotroph* getChild(RandomSimple&, const HeterotrophProcessor&);

  double consumePreyVolume(const double);
  double metabolise(const double);

  Traits& getHeritableTraits();
  double getTrophicLevel() const;
  unsigned getSizeClassIndex() const;
  unsigned getAge() const;

  bool hasFed() const;
  bool isDead() const;

  double getVolumeActual() const;
  double getVolumeHeritable() const;
  double getVolumeMinimum() const;
  double getVolumeReproduction() const;

  double getStarvationMultiplier() const;

  void setTrophicLevel(const double);
  void setSizeClassIndex(const unsigned);

  void setAge(const unsigned);
  void setHasFed(const bool);
  void setDead();

 private:
  Traits traits_;

  double assimilationEfficiency_;

  double volumeHeritable_;
  double volumeMinimum_;
  double volumeReproduction_;

  double volumeActual_;
  double trophicLevel_;

  double starvationMultiplier_;

  unsigned sizeClassIndex_;
  unsigned age_;

  bool hasFed_;
  bool isDead_;
};

#endif
