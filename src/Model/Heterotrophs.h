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

  void update();
  bool recordData();

  Heterotroph* getIndividual(const unsigned, const unsigned);
  std::size_t getSizeClassPopulation(const unsigned) const;

 private:
  void createInitialPopulation();
  void calculateFeedingProbabilities();

  void feeding();
  void metabolisation();
  void starvation();
  void reproduction();

  Heterotroph* getRandomIndividualFromSizeClass(const unsigned);
  Heterotroph* getRandomPredatorFromSizeClass(const unsigned);
  Heterotroph* getRandomPreyFromSizeClass(const unsigned, const Heterotroph*);

  void feedFromAutotrophs(Heterotroph*);
  void feedFromHeterotrophs(Heterotroph*, const unsigned);

  void stageForMoving(Heterotroph*, const unsigned);
  void moveSizeClass(Heterotroph*, const unsigned);
  bool removeFromSizeClass(Heterotroph*, const unsigned);
  void deleteIndividual(Heterotroph*);

  void starve(Heterotroph*);
  void kill(Heterotroph*);

  void addChildren();
  void moveIndividuals();
  void deleteDead();

  Nutrient& nutrient_;
  Autotrophs& autotrophs_;

  HeterotrophProcessor heterotrophProcessor_;
  HeterotrophData heterotrophData_;

  RandomSimple random_;
  Timer timer_;

  const std::vector<std::vector<double>> interSizeClassPreferenceMatrix_;
  const std::vector<std::vector<double>> interSizeClassVolumeMatrix_;

  const std::vector<double> sizeClassMidPoints_;
  const std::vector<unsigned> maximumSizeClassPopulations_;

  const double smallestIndividualVolume_;
  const double initialHeterotrophicVolume_;
  const double sizeClassSubsetFraction_;

  const unsigned numberOfSizeClasses_;
  const unsigned preferredPreyVolumeRatio_;
  const unsigned autotrophSizeClassIndex_;

  std::vector<unsigned> fedCount_;

  std::vector<Heterotroph*> children_;
  std::vector<std::pair<Heterotroph*, unsigned>> individualsToMove_;

  std::vector<std::vector<Heterotroph*>> individuals_;
  std::vector<std::vector<Heterotroph*>> individualsDead_;
};

#endif
