#ifndef HETEROTROPH
#define HETEROTROPH

#include "Traits.h"

class HeterotrophProcessor;
class RandomSimple;

class Heterotroph {
 public:
  Heterotroph() = delete;
  explicit Heterotroph(const Traits&, const double);
  explicit Heterotroph(const Traits&, const double, const double, const double, const double);

  Heterotroph(const Heterotroph&);
  Heterotroph(const Heterotroph&&) noexcept;

  Heterotroph& operator=(const Heterotroph&);
  Heterotroph& operator=(const Heterotroph&&);

  Heterotroph* getChild(RandomSimple&, const HeterotrophProcessor&);

  double consumePreyVolume(const double);
  double metabolise(const double);

  Traits& getHeritableTraits();
  double getTrophicLevel() const;
  unsigned getAge() const;

  bool hasFed() const;
  bool isDead() const;

  double getVolumeActual() const;
  double getVolumeHeritable() const;
  double getVolumeMinimum() const;
  double getVolumeReproduction() const;

  double getStarvationMultiplier() const;

  void setTrophicLevel(const double);

  void setAge(const unsigned);
  void setHasFed(const bool);
  void setDead();

 private:
  Traits traits_;
  double volumeHeritable_;

  double volumeActual_;
  double volumeMinimum_;
  double volumeReproduction_;

  double assimilationEfficiency_;
  double starvationMultiplier_;

  double trophicLevel_;
  unsigned age_;
  bool hasFed_;
  bool isDead_;
};

#endif
