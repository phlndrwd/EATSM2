#ifndef AUTOTROPHS
#define AUTOTROPHS

class Nutrient;

class Autotrophs {
 public:
  Autotrophs(Nutrient&);
  ~Autotrophs();

  void recordData();

  void update();

  double getVolume();
  void setVolume(const double);

  void addToVolume(const double);
  void subtractFromVolume(const double);

 private:
  Nutrient& nutrient_;

  double volume_;
  double toFlux_;
};

#endif
