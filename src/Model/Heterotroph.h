#ifndef HETEROTROPH
#define HETEROTROPH

#include "HeritableTraits.h"
#include "HeterotrophProcessor.h"
#include "RandomSimple.h"
#include "Types.h"

class Heterotroph {
 public:
  Heterotroph(const HeritableTraits&, const double, const unsigned);
  Heterotroph(const HeritableTraits&, const double, const double, const double, const double, const unsigned);
  Heterotroph(const HeritableTraits&, const double, const double, const unsigned);
  Heterotroph(const HeritableTraits&, const double, const double, const double, const double, const double,
              const double, const unsigned, const unsigned, const bool, const bool);
  Heterotroph(const Heterotroph&);
  Heterotroph(const Heterotroph&&) noexcept;
  ~Heterotroph();
  Heterotroph& operator=(const Heterotroph&);
  bool operator==(const Heterotroph&);

  Types::HeterotrophPointer Reproduce(RandomSimple&, HeterotrophProcessor&);

  double ConsumePreyVolume(const double);
  double Metabolise(const double);

  HeritableTraits& GetHeritableTraits();
  double GetTrophicLevel() const;
  unsigned GetSizeClassIndex() const;
  unsigned GetAge() const;

  bool HasFed() const;
  bool IsDead() const;

  double GetVolumeActual() const;
  double GetVolumeHeritable() const;
  double GetVolumeMinimum() const;
  double GetVolumeReproduction() const;

  double GetStarvationMultiplier() const;

  void SetTrophicLevel(const double);
  void SetSizeClassIndex(const unsigned);

  void SetAge(const unsigned);
  void SetHasFed(const bool);
  void Kill();

 private:
  HeritableTraits mHeritableTraits;

  const double mAssimilationEfficiency;

  double mVolumeHeritable;
  double mVolumeMinimum;
  double mVolumeReproduction;

  double mVolumeActual;
  double mTrophicLevel;

  double mStarvationMultiplier;

  unsigned mSizeClassIndex;
  unsigned mAge;

  bool mHasFed;
  bool mIsDead;
};

#endif
