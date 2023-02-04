#ifndef NUTRIENT
#define NUTRIENT

class Nutrient {
 public:
  Nutrient();

  void recordData();

  const double& getVolume() const;

  void addToVolume(const double);
  void subtractFromVolume(const double);

 private:
  double volume_;
  double toFlux_;
};

#endif
