#ifndef AUTOTROPHS
#define AUTOTROPHS

class Nutrient;

class Autotrophs {
 public:
  Autotrophs() = delete;
  explicit Autotrophs(Nutrient&, const double);

  void update();
  void addToVolume(const double);
  void subtractFromVolume(const double);
  void recordData();

  double& getVolume();
  const double& getVolume() const;

 private:
  Nutrient& nutrient_;

  double volume_;
  double toFlux_;
};

#endif
