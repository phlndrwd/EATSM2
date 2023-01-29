#ifndef HETEROTROPHS
#define HETEROTROPHS

#include "HeterotrophData.h"
#include "HeterotrophProcessor.h"
#include "RandomSimple.h"
#include "Timer.h"

class Autotrophs;
class Nutrient;

class Heterotrophs {
 public:
  Heterotrophs(Nutrient&, Autotrophs&);
  ~Heterotrophs();

  void Update();
  bool RecordData();

  Heterotroph* GetIndividual(const unsigned, const unsigned);
  std::size_t GetSizeClassPopulation(const unsigned) const;

 private:
  void CreateInitialPopulation();
  void CalculateFeedingProbabilities();

  void Feeding();
  void Metabolisation();
  void Starvation();
  void Reproduction();

  Heterotroph* GetRandomIndividualFromSizeClass(const unsigned);
  Heterotroph* GetRandomPredatorFromSizeClass(const unsigned);
  Heterotroph* GetRandomPreyFromSizeClass(const unsigned, const Heterotroph*);

  void FeedFromAutotrophs(Heterotroph*);
  void FeedFromHeterotrophs(Heterotroph*, const unsigned);

  void StageForMoving(Heterotroph*, const unsigned);
  void MoveSizeClass(Heterotroph*, const unsigned);
  bool RemoveFromSizeClass(Heterotroph*, const unsigned);
  void DeleteIndividual(Heterotroph*);

  void Starve(Heterotroph*);
  void Kill(Heterotroph*);

  void AddChildren();
  void MoveIndividuals();
  void DeleteDead();

  Nutrient& mNutrient;
  Autotrophs& mAutotrophs;

  HeterotrophProcessor mHeterotrophProcessor;
  HeterotrophData mHeterotrophData;

  RandomSimple mRandom;
  Timer mTimer;

  const std::vector<std::vector<double>> mInterSizeClassPreferenceMatrix;
  const std::vector<std::vector<double>> mInterSizeClassVolumeMatrix;

  const std::vector<double> mSizeClassMidPoints;
  const std::vector<unsigned> mMaximumSizeClassPopulations;

  const double mSmallestIndividualVolume;
  const double mInitialHeterotrophicVolume;
  const double mSizeClassSubsetFraction;

  const unsigned mNumberOfSizeClasses;
  const unsigned mPreferredPreyVolumeRatio;
  const unsigned mAutotrophSizeClassIndex;

  std::vector<unsigned> mFedCount;

  std::vector<Heterotroph*> mChildren;
  std::vector<std::pair<Heterotroph*, unsigned>> mIndividualsToMove;

  std::vector<std::vector<Heterotroph*>> mIndividuals;
  std::vector<std::vector<Heterotroph*>> mIndividualsDead;
};

#endif
